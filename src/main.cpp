#include "app/app.hpp"
#include "game/config.hpp"
#include "game/map.hpp"
#include "game/monster.hpp"
#include "game/signal_defs.hpp"

//! @brief tag component for backpack panel UI
struct BackpackUIPanel {};

struct NearestItemHover {
    bool valid = false;
    math::Vector2 position;
};

struct Context {
    FontHandle font;
    bool showDebugInfo = false;
};

void LoadResourceSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& renderer = resources.Get<Renderer>();
    auto& assets = resources.Get<AssetsManager>();
    cmd.SetResource<ImageHandle>(assets.Image().Load("resources/img/role.png"))
        .SetResource<Context>(Context{FontHandle::Null()});

    auto& manager = resources.Get<TilesheetManager>();
    manager.LoadFromConfig("resources/img/tilesheet.lua");
    manager.LoadFromConfig("resources/img/role.lua");
    LOGI("resources loaded");
}

enum GameState {
    Menu,
    Game,
} state;

void ReadConfigSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& luaMgr = resources.Get<AssetsManager>().Lua();
    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    GameConfig config(luaMgr, tilesheetMgr, "./resources/config/");
    if (!config) {
        LOGF("Load config in ./resources/config failed!!! Game can't start!!!");
        exit(1);
    }
    cmd.SetResource<GameConfig>(std::move(config));
    LOGI("config readed");
}

void InitMapSystem(ecs::Commands& cmd, ecs::Resources resources) {
    MapManager mapMgr;
    mapMgr.Add(GenDebugDemo(resources, 32, 23));

    cmd.SetResource<MapManager>(std::move(mapMgr));
}

void InitMonstersSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    auto& tilesheet = tilesheetMgr.Find("role");

    auto downTile = tilesheet.Get(0, 0);
    auto rightTile = tilesheet.Get(1, 0);
    auto upTile = tilesheet.Get(2, 0);

	auto monster = Monster::CreateMonster(
        SpriteBundle{Sprite::FromRegion(rightTile.region), rightTile.handle},
        SpriteBundle{Sprite::FromRegion(upTile.region), upTile.handle},
        SpriteBundle{Sprite::FromRegion(downTile.region), downTile.handle});
    cmd.Spawn<Monster, Player, Backpack>(std::move(monster), Player{}, Backpack{});
}

void InitBackpackUISystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& config = resources.Get<GameConfig>();
    auto& window = resources.Get<Window>();
    auto& backpackUIConfig = config.GetBackpackUIConfig().Info();
    auto backpackPanel = cmd.SpawnAndReturn<Node, ui::Panel, ui::RectTransform, BackpackUIPanel>(
		Node {},
        ui::Panel::Create(
            ui::ColorBundle::CreatePureColor(Color{200, 200, 200}),
            ui::ColorBundle::CreatePureColor(Color::Black)),
        ui::RectTransform{NodeTransform{Transform::FromPosition({0, window.GetSize().y - backpackUIConfig.height})},
                          math::Vector2(backpackUIConfig.width, backpackUIConfig.height)},
        {});

    auto& signalMgr = resources.Get<SignalManager>();
    signalMgr.Regist(
        SignalCallback::UpdateBackpackItem,
        [](ecs::Commands& cmds, ecs::Querier querier, ecs::Resources resources,
           ecs::Events&) {
            auto entities = querier.Query<BackpackUIPanel>();
            if (entities.empty()) return;

            auto entity = entities[0];

            entities = querier.Query<Player>();
            if (entities.empty()) return;
            auto backpackEntity = entities[0];
            auto& backpack = querier.Get<Backpack>(backpackEntity);

            // uint32_t* newItemCount = (uint32_t*)param;
            uint32_t* newItemCount = nullptr;

            auto& panel = querier.Get<ui::Panel>(entity);
            auto& node = querier.Get<Node>(entity);
            auto& backpackUIConfig =
                resources.Get<GameConfig>().GetBackpackUIConfig().Info();
            // we assume newItemCount == 1, will fix this later
            int size = node.children.size() + *newItemCount - 1;
            int x = size % backpackUIConfig.col;
            int y = size / backpackUIConfig.col;

            const auto& item = backpack.items.back();
            const auto& itemConfig = resources.Get<GameConfig>().GetItemConfig()->Items();
            auto it = itemConfig.find(backpack.items.back().nameID);
            if (it == itemConfig.end()) {
                LOGW("item ", item.nameID, " don't in config file");
                return;
            }
            const auto& itemInfo = it->second;

            node.children.push_back(cmds.SpawnImmediateAndReturn<Node, ui::Panel,
                                                       ui::RectTransform,
                                                       ui::Image>(
                Node {entity},
                ui::Panel::Create(
                    ui::ColorBundle::CreatePureColor(Color{70, 70, 70}),
                    ui::ColorBundle::CreatePureColor(Color::Black)),
                ui::RectTransform{
                    NodeTransform{Transform::FromPosition(math::Vector2(
                        backpackUIConfig.margin + (backpackUIConfig.gridSize +
                                                   backpackUIConfig.padding) *
                                                      x,
                        backpackUIConfig.margin + (backpackUIConfig.gridSize +
                                                   backpackUIConfig.padding) *
                                                      y))},
                    math::Vector2(backpackUIConfig.gridSize,
                                  backpackUIConfig.gridSize)},
                    ui::Image::FromSpriteBundle(itemInfo.sprite)));
        });
}

void StartupSystem(ecs::Commands& cmd, ecs::Resources resources) {
    cmd.SetResource<NearestItemHover>(NearestItemHover{});
    ReadConfigSystem(cmd, resources);
    InitMapSystem(cmd, resources);
    InitMonstersSystem(cmd, resources);
    InitBackpackUISystem(cmd, resources);
    LOGI("game startup");
}

void PlayerMove(Keyboard& keyboard, Monster& monster) {
    constexpr float SPEED = 2;

    math::Vector2 velocity;
    
    if (keyboard.Key(SDL_SCANCODE_D).IsPressing() || keyboard.Key(SDL_SCANCODE_A).IsPressing()) {
        if (keyboard.Key(SDL_SCANCODE_D).IsPressing()) {
            velocity.Set(1.0, 0.0);
        } else {
            velocity.Set(-1.0, 0.0);
        }
    }
    if (keyboard.Key(SDL_SCANCODE_W).IsPressing() || keyboard.Key(SDL_SCANCODE_S).IsPressing()) {
        if (keyboard.Key(SDL_SCANCODE_S).IsPressing()) {
            velocity += math::Vector2(0, 1);
        } else {
            velocity += math::Vector2(0, -1);
        }
    }

    velocity.Normalize();
    velocity *= SPEED;
    monster.Move(velocity);
}

void ToggleBackpackUIPanel(ecs::Querier querier, ecs::Resources res) {
    auto entities = querier.Query<BackpackUIPanel>();
    if (!entities.empty()) {
        auto& window = res.Get<Window>();
        auto& transform = querier.Get<ui::RectTransform>(entities[0]);
        float& y = transform.transform.localTransform.position.y;
        if (y >= 0 && y < window.GetSize().y) { // panel in windows, move it out
            y = window.GetSize().y;
        } else {
            auto& info = res.Get<GameConfig>().GetBackpackUIConfig().Info();
            y = window.GetSize().y - info.height;
        }
    }
}

void DetectNearestItem(ecs::Commands& cmd, ecs::Querier querier,
                       ecs::Resources resources, ecs::Events& events) {
    auto entities = querier.Query<Player>();
    if (entities.empty()) {
        return;
    }

    auto entity = entities[0];
    auto& monster = querier.Get<Monster>(entity);
    auto& nearestItem = resources.Get<NearestItemHover>();

    constexpr float PickupHalfRange = 100;
    auto& map = resources.Get<MapManager>().GetCurrentMap();
    math::Rect pickupArea =
        math::Rect::Create(monster.position.x - PickupHalfRange,
                        monster.position.y - PickupHalfRange,
                        PickupHalfRange * 2.0, PickupHalfRange * 2.0);
    pickupArea = math::Rect::Create(
        pickupArea.x / MapTileRealSize, pickupArea.y / MapTileRealSize,
        pickupArea.w / MapTileRealSize, pickupArea.h / MapTileRealSize);
    // FIXME: monster should use it's center position
    auto monsterPos = monster.position;
    int left = std::ceil(pickupArea.x);
    int bottom = std::ceil(pickupArea.y);
    int right = std::min<float>(map->tiles.Width(), left + std::floor(pickupArea.w));
    int top = std::min<float>(map->tiles.Height(), bottom + std::floor(pickupArea.h));
    left = std::max(0, left);
    bottom = std::max(0, bottom);
    double minDistSqrd = std::numeric_limits<double>::max();
    for (int x = left; x < right; x++) {
        for (int y = bottom; y < top; y++) {
            math::Vector2 tileCenter((x + 0.5) * MapTileRealSize, (y + 0.5) * MapTileRealSize);
            auto& tile = map->tiles.Get(x, y);
            double distSqrd = math::LengthSqrd(tileCenter - monsterPos);
            if (!tile.items.empty() &&
                distSqrd < minDistSqrd &&
                distSqrd <= PickupHalfRange * PickupHalfRange) {
                minDistSqrd = distSqrd;
                nearestItem.position.Set(x, y);
                nearestItem.valid = true;
            }
        }
    }

    if (minDistSqrd == std::numeric_limits<double>::max()) {
        nearestItem.valid = false;
    }
}

void DrawNearestItemPointer(ecs::Commands& cmd, ecs::Querier querier,
                          ecs::Resources resources, ecs::Events& events) {
    auto& hover = resources.Get<NearestItemHover>();
    if (!hover.valid) return;

    auto& map = resources.Get<MapManager>().GetCurrentMap();
    auto& renderer = resources.Get<Renderer>();
    if (map->tiles.IsInRange(hover.position.x, hover.position.y)) {
        auto& tilesheet = resources.Get<TilesheetManager>().Find("tilesheet");
        auto tile = tilesheet.Get(4, 10);
        SpriteBundle sprite;
        sprite.image = tile.handle;
        sprite.sprite = Sprite::FromRegion(tile.region);
        sprite.sprite.customSize.Set(MapTileRealSize, MapTileRealSize);
        renderer.DrawSprite(sprite, Transform::FromPosition(hover.position * MapTileRealSize));
    }
}

void PickupItemOnTile(Monster& monster, Backpack& backpack, ecs::Commands& cmd,
                      ecs::Querier querier, ecs::Resources resources,
                      ecs::Events& events) {
    constexpr float PickupHalfRange = 100;
    auto& map = resources.Get<MapManager>().GetCurrentMap();
    auto& hover = resources.Get<NearestItemHover>();
    if (map->tiles.IsInRange(hover.position.x, hover.position.y) &&
        !map->tiles.Get(hover.position.x, hover.position.y).items.empty()) {
        auto& items = map->tiles.Get(hover.position.x, hover.position.y).items;
        backpack.items.push_back(items.back());
        items.pop_back();

        uint32_t newItemCount = 1;
        auto& signalMgr = resources.Get<SignalManager>();
        signalMgr.Raise(SignalCallback::UpdateBackpackItem, cmd,
                        querier, resources, events);
    }
}

void InputHandle(ecs::Commands& cmd, ecs::Querier querier,
                 ecs::Resources resources, ecs::Events& events) {
    auto& keyboard = resources.Get<Keyboard>();

    auto entities = querier.Query<ecs::With<Player, Monster>>();
    for (auto entity : entities) {
        auto& monster = querier.Get<Monster>(entity);
        PlayerMove(keyboard, monster);


        if (keyboard.Key(SDL_SCANCODE_SPACE).IsPressed() && querier.Has<Backpack>(entity)) {
            auto& backpack = querier.Get<Backpack>(entity);
            PickupItemOnTile(monster, backpack, cmd, querier, resources, events);
        }
    }

    if (keyboard.Key(SDL_SCANCODE_TAB).IsPressed()) {
        ToggleBackpackUIPanel(querier, resources);
    }
}

void InitLuaScript(ecs::Commands& cmds, ecs::Resources res) {
    auto& luaMgr = res.Get<AssetsManager>().Lua();
    cmds.Spawn<Script>(Script {
        luaMgr.CreateSolitary("resources/script/test.lua")
    });
}

class GameApp final : public App {
public:
    GameApp() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
            .AddStartupSystem(LoadResourceSystem)
            .AddStartupSystem(StartupSystem)
            .AddStartupSystem(InitLuaScript)
            .AddSystem(DetectNearestItem)
            .AddSystem(InputHandle)
            .AddSystem(MonsterUpdate)
            .AddSystem(DrawMapSystem)
            .AddSystem(DrawMonsterSystem)
            .AddSystem(DrawNearestItemPointer)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(GameApp)