#include "app/app.hpp"
#include "game/config.hpp"
#include "game/map.hpp"
#include "game/monster.hpp"

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

void StartupSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& luaMgr = resources.Get<LuaManager>();
    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    GameConfig config(luaMgr, tilesheetMgr, "./resources/config/");
    Random::Init();

    if (!config.Valid()) {
        LOGF("Load config in ./resources/config failed!!! Game can't start!!!");
        exit(1);
    }

    cmd.SetResource<GameConfig>(std::move(config));

    MapManager mapMgr;
    mapMgr.Add(GenDebugDemo(resources, 32, 23));

    cmd.SetResource<MapManager>(std::move(mapMgr));

    auto& tilesheet = tilesheetMgr.Find("role");

    auto downTile = tilesheet.Get(0, 0);
    auto rightTile = tilesheet.Get(1, 0);
    auto upTile = tilesheet.Get(2, 0);

	auto monster = Monster::CreateMonster(
        SpriteBundle{Sprite::FromRegion(rightTile.region), rightTile.handle},
        SpriteBundle{Sprite::FromRegion(upTile.region), upTile.handle},
        SpriteBundle{Sprite::FromRegion(downTile.region), downTile.handle});

    auto entity = cmd.Spawn<Monster, Player>(std::move(monster), Player{});
}

void InputHandle(ecs::Commands& cmd, ecs::Querier queryer,
                 ecs::Resources resources, ecs::Events& events) {
    constexpr float SPEED = 2;

    auto& keyboard = resources.Get<Keyboard>();

    auto entities = queryer.Query<Player>();
    for (auto entity : entities) {
        auto& monster = queryer.Get<Monster>(entity);

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