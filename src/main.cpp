#include "app/app.hpp"
#include "game/config.hpp"
#include "game/map.hpp"
#include "game/monster.hpp"

//! @brief tag component for backpack panel UI
struct BackpackUIPanel {};

struct Context {
    FontHandle font;
    bool showDebugInfo = false;
};

void LoadResourceSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& renderer = resources.Get<Renderer>();
    auto& assets = resources.Get<AssetsManager>();
    cmd.SetResource<ImageHandle>(assets.Image().Load("resources/img/role.png"))
        .SetResource<Context>(
            Context{assets.Font().Load("resources/font/SimHei.ttf", 20)});

    auto& manager = resources.Get<TilesheetManager>();
    manager.LoadFromConfig("resources/img/tilesheet.lua");
    manager.LoadFromConfig("resources/img/role.lua");
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
    auto backpackPanel = cmd.SpawnAndReturn<ui::Panel, ui::RectTransform, BackpackUIPanel>(
        ui::Panel::Create(
            ui::ColorBundle::CreatePureColor(Color{200, 200, 200}),
            ui::ColorBundle::CreatePureColor(Color::Black)),
        ui::RectTransform{NodeTransform{Transform::FromPosition({0, window.GetSize().y - backpackUIConfig.height})},
                          math::Vector2(backpackUIConfig.width, backpackUIConfig.height)},
        {});
    std::vector<ecs::Entity> grides;
    for (int x = 0; x < backpackUIConfig.col; x++) {
        for (int y = 0; y < backpackUIConfig.row; y++) {
            auto entity = cmd.SpawnAndReturn<ui::Panel, ui::RectTransform>(
                ui::Panel::Create(
                    ui::ColorBundle::CreatePureColor(Color{70, 70, 70}),
                    ui::ColorBundle::CreatePureColor(Color::Black)),
                ui::RectTransform{
                    NodeTransform{Transform::FromPosition(
                        math::Vector2(backpackUIConfig.margin + (backpackUIConfig.gridSize +
                                                    backpackUIConfig.padding) *
                                                       x,
                         backpackUIConfig.margin + (backpackUIConfig.gridSize +
                                                    backpackUIConfig.padding) *
                                                       y))},
                    math::Vector2(backpackUIConfig.gridSize,
                                  backpackUIConfig.gridSize)});
            grides.push_back(entity);
        }
    }

    HierarchyBuilder builder(cmd, backpackPanel);
    builder.SetChilds(grides);

    cmd.SetResource<GameConfig>(std::move(config));
}

void StartupSystem(ecs::Commands& cmd, ecs::Resources resources) {
    ReadConfigSystem(cmd, resources);
    InitMapSystem(cmd, resources);
    InitMonstersSystem(cmd, resources);
    InitBackpackUISystem(cmd, resources);
}

void InputHandle(ecs::Commands& cmd, ecs::Querier querier,
                 ecs::Resources resources, ecs::Events& events) {
    constexpr float SPEED = 2;

    auto& keyboard = resources.Get<Keyboard>();

    auto entities = querier.Query<Player>();
    for (auto entity : entities) {
        auto& monster = querier.Get<Monster>(entity);

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
        if (keyboard.Key(SDL_SCANCODE_TAB).IsPressed()) {
            auto entities = querier.Query<BackpackUIPanel>();
            if (!entities.empty()) {
                auto& window = resources.Get<Window>();
                auto& transform = querier.Get<ui::RectTransform>(entities[0]);
                float& y = transform.transform.localTransform.position.y;
                if (y >= 0 && y < window.GetSize().y) { // panel in windows, move it out
                    y = window.GetSize().y;
                } else {
                    auto& info = resources.Get<GameConfig>().GetBackpackUIConfig().Info();
                    y = window.GetSize().y - info.height;
                }
            }
        }

        velocity.Normalize();
        velocity *= SPEED;
        monster.Move(velocity);
    }
}

void ShowDebugInfoSystem(ecs::Commands&, ecs::Querier, ecs::Resources resources,
                         ecs::Events&) {
    auto& context = resources.Get<Context>();
    auto& renderer = resources.Get<Renderer>();
    auto& timer = resources.Get<Time>();
    auto& keyboard = resources.Get<Keyboard>();

    if (keyboard.Key(SDL_SCANCODE_G).IsPressed()) {
        context.showDebugInfo = !context.showDebugInfo;
    }

    if (context.showDebugInfo) {
        renderer.SetDrawColor({255, 255, 255});
        Transform transform;
        transform.position = {0, 0};
        renderer.DrawText(context.font,
                          "fps: " + std::to_string(static_cast<uint32_t>(
                                        1000.0 / timer.Elapse())),
                          transform);
    }
}

class GameApp final : public App {
public:
    GameApp() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
            .AddStartupSystem(LoadResourceSystem)
            .AddStartupSystem(StartupSystem)
            .AddSystem(InputHandle)
            .AddSystem(MonsterUpdate)
            .AddSystem(DrawMapSystem)
            .AddSystem(DrawMonsterSystem)
            .AddSystem(ShowDebugInfoSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(GameApp)