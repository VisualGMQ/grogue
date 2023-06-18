#include "app/app.hpp"
#include "game/config.hpp"
#include "game/map.hpp"
#include "game/monster.hpp"
#include "game/signal_defs.hpp"
#include "game/game.hpp"
#include "game/input.hpp"

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

void InitPhysicsSystem(ecs::Resources res) {
    auto& physicWorld = res.Get<physic::PhysicWorld>();
    physicWorld.forceGenerators.push_back([](physic::Particle& p, Time::TimeType){
        if (p.vel.LengthSqrd() > 0.000001) {
            constexpr float FrictionFactor = 6.0;
            p.force += -math::Normalize(p.vel) * 1.0 / p.massInv * FrictionFactor;
        }
    });
}

void InitInputSystem(ecs::Commands& cmd, ecs::Resources res) {
#if defined(GROGUE_PLATFORM_ANDROID) || defined(GROGUE_PLATFORM_APPLE) // Touch devices
    InputPtr input = std::make_unique<TouchInput>();
#else   // Keyboard devices
    auto& luaMgr = res.Get<AssetsManager>().Lua();
    auto lua = luaMgr.CreateSolitary("./resources/config/game_conf.lua");
    sol::table keyboardConfig = lua.lua["Config"]["keyboard_input"];
    std::unordered_map<std::string, Key> actions;
    for (auto& [name, value] : keyboardConfig) {
        actions[name.as<std::string>()] = static_cast<Key>(SDL_GetScancodeFromName(value.as<std::string>().c_str()));
    }
    InputPtr input = std::make_unique<KeyboardInput>(res.Get<Keyboard>(), std::move(actions));
#endif
    cmd.SetResource<InputPtr>(std::move(input));
}

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

void InitMapSystem(ecs::Commands& cmds, ecs::Resources resources) {
    MapManager mapMgr;
    mapMgr.Add(GenDebugDemo(cmds, resources, 32, 23));

    cmds.SetResource<MapManager>(std::move(mapMgr));
}

void InitMonstersSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    auto& tilesheet = tilesheetMgr.Find("role");
    auto& luaMgr = resources.Get<AssetsManager>().Lua();

    auto downTile = tilesheet.Get(0, 0);
    auto rightTile = tilesheet.Get(1, 0);
    auto upTile = tilesheet.Get(2, 0);

	auto monster = Monster::CreateMonster(
        SpriteBundle{Sprite::FromRegion(rightTile.region), rightTile.handle},
        SpriteBundle{Sprite::FromRegion(upTile.region), upTile.handle},
        SpriteBundle{Sprite::FromRegion(downTile.region), downTile.handle});
        cmd.Spawn<Monster, Player, Backpack, physic::Particle, physic::CollideShape, Script>(
            std::move(monster), Player{}, Backpack{},
            physic::Particle::Create(math::Vector2::Zero, 1.0),
            physic::CollideShape{std::make_shared<physic::AABB>(math::Vector2{16, 34}, math::Vector2{12, 12})},
            Script::Create(luaMgr, "resources/script/player.lua"));
}

void InitBackpackUISystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& config = resources.Get<GameConfig>();
    auto& window = resources.Get<Window>();
    auto& backpackUIConfig = config.GetBackpackUIConfig().Info();
    auto& luaMgr = resources.Get<AssetsManager>().Lua();
    auto backpackPanel = cmd.SpawnAndReturn<Node, ui::Panel, ui::RectTransform,
                                            BackpackUIPanel>(
        Node{},
        ui::Panel::Create(
            ui::ColorBundle::CreatePureColor(Color{200, 200, 200}),
            ui::ColorBundle::CreatePureColor(Color::Black)),
        ui::RectTransform{
            NodeTransform{Transform::FromPosition(
                {0, window.GetSize().y - backpackUIConfig.height})},
            math::Vector2(backpackUIConfig.width, backpackUIConfig.height)},
        {});

    auto& signalMgr = resources.Get<SignalManager>();
    signalMgr.Regist(
        SignalCallback::UpdateBackpackItem,
        [](ecs::Commands& cmds, ecs::Querier querier, ecs::Resources resources,
           ecs::Events&, void* param) {
            auto entities = querier.Query<BackpackUIPanel>();
            if (entities.empty()) return;

            auto entity = entities[0];

            entities = querier.Query<Player>();
            if (entities.empty()) return;
            auto backpackEntity = entities[0];
            auto& backpack = querier.Get<Backpack>(backpackEntity);

            // uint32_t* newItemCount = (uint32_t*)param;
            uint32_t newItemCount = 1;

            auto& panel = querier.Get<ui::Panel>(entity);
            auto& node = querier.Get<Node>(entity);
            auto& backpackUIConfig =
                resources.Get<GameConfig>().GetBackpackUIConfig().Info();
            // FIXME: we assume newItemCount == 1, will fix this later
            int size = node.children.size() + newItemCount - 1;
            int x = size % backpackUIConfig.col;
            int y = size / backpackUIConfig.col;

            const auto& item = backpack.items.back();
            const auto& itemConfig =
    resources.Get<GameConfig>().GetItemConfig().Items(); auto it =
    itemConfig.find(backpack.items.back().nameID); if (it == itemConfig.end()) {
                LOGW("item ", item.nameID, " don't in config file");
                return;
            }
            const auto& itemInfo = it->second;

            node.children.push_back(cmds.SpawnImmediateAndReturn<Node,
    ui::Panel, ui::RectTransform, ui::Image>( Node {entity}, ui::Panel::Create(
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
    InitPhysicsSystem(resources);
    InitInputSystem(cmd, resources);
    InitMapSystem(cmd, resources);
    InitMonstersSystem(cmd, resources);
    InitBackpackUISystem(cmd, resources);
    LOGI("game startup");
}

void DetectNearestItem(ecs::Commands& cmd, ecs::Querier querier,
                       ecs::Resources resources, ecs::Events& events) {
    auto entities = querier.Query<Player>();
    if (entities.empty()) {
        return;
    }

    auto entity = entities[0];
    auto& monster = querier.Get<Monster>(entity);
    auto& particle = querier.Get<physic::Particle>(entity);
    auto& nearestItem = resources.Get<NearestItemHover>();

    constexpr float PickupHalfRange = 100;
    auto& map = resources.Get<MapManager>().GetCurrentMap();
    auto monsterPos = particle.pos;
    math::Rect pickupArea =
        math::Rect::Create(monsterPos.x - PickupHalfRange,
                           monsterPos.y - PickupHalfRange,
                           PickupHalfRange * 2.0, PickupHalfRange * 2.0);
    pickupArea = math::Rect::Create(
        pickupArea.x / MapTileRealSize, pickupArea.y / MapTileRealSize,
        pickupArea.w / MapTileRealSize, pickupArea.h / MapTileRealSize);
    // FIXME: monster should use it's center position
    int left = std::ceil(pickupArea.x);
    int bottom = std::ceil(pickupArea.y);
    int right =
        std::min<float>(map->tiles.Width(), left + std::floor(pickupArea.w));
    int top =
        std::min<float>(map->tiles.Height(), bottom + std::floor(pickupArea.h));
    left = std::max(0, left);
    bottom = std::max(0, bottom);
    double minDistSqrd = std::numeric_limits<double>::max();
    for (int x = left; x < right; x++) {
        for (int y = bottom; y < top; y++) {
            math::Vector2 tileCenter((x + 0.5) * MapTileRealSize,
                                     (y + 0.5) * MapTileRealSize);
            auto tileEntity = map->tiles.Get(x, y);
            auto& tile = querier.Get<MapTile>(tileEntity);
            double distSqrd = math::LengthSqrd(tileCenter - monsterPos);
            if (!tile.items.empty() && distSqrd < minDistSqrd &&
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
        renderer.DrawSprite(
            sprite, Transform::FromPosition(hover.position * MapTileRealSize));
    }
}

class GameApp final : public App {
public:
    GameApp() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
            .AddStartupSystem(LoadResourceSystem)
            .AddStartupSystem(StartupSystem)
            .AddSystem(DetectNearestItem)
            .AddSystem(DrawMapSystem)
            .AddSystem(DrawMonsterSystem)
            .AddSystem(DrawNearestItemPointer)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(GameApp)