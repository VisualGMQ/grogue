#include "app/app.hpp"
#include "game/config.hpp"

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

    auto& manager = resources.Get<TileSheetManager>();
    manager.LoadFromConfig("resources/img/tilesheet_tile.lua");
}

enum GameState {
    Menu,
    Game,
} state;

void StartupSystem(ecs::Commands& cmd, ecs::Resources resources) {
    GameConfig config("./resources/config/");
    if (!config.Valid()) {
        LOGF("Load config in ./resources/config failed!!! Game can't start!!!");
        exit(1);
    }

    cmd.SetResource<GameConfig>(std::move(config));

    Transform transform;
    transform.position = {100, 100};

    auto& handle = resources.Get<ImageHandle>();
    Sprite sprite = Sprite::Default();
    auto entity1 = cmd.SpawnAndReturn<SpriteBundle, NodeTransform>(SpriteBundle{sprite, handle}, NodeTransform{transform, {}});
    transform.position.x = 40;
    transform.scale = {2, 2};
    transform.rotation = 30;
    sprite.color = {255, 0, 0};
    auto entity2 = cmd.SpawnAndReturn<SpriteBundle, NodeTransform>(SpriteBundle{sprite, handle}, NodeTransform{transform, {}});
    transform.position.x = 80;
    transform.scale = {1, 1};
    sprite.color = {0, 255, 0};
    auto entity3 = cmd.SpawnAndReturn<SpriteBundle, NodeTransform>(SpriteBundle{sprite, handle}, NodeTransform{transform, {}});
    transform.position.x = 120;
    sprite.color = {0, 0, 255};
    transform.scale = {2, 2};
    auto entity4 = cmd.SpawnAndReturn<SpriteBundle, NodeTransform>(SpriteBundle{sprite, handle}, NodeTransform{transform, {}});

    /*
        root 
        /  \
    child1 child2
       |
       leaf1
    */

    Node root;
    root.SetEntity(entity1)
        .AddChild(
            Node(entity2, {
                Node(entity3, {})
            })
        .AddChild(
            Node(entity4, {})
        ));

    auto& scene = resources.Get<Scene>();
    scene.root = root;
}

void InputHandle(ecs::Commands& cmd, ecs::Querier queryer,
                 ecs::Resources resources, ecs::Events& events) {
    auto& keyboard = resources.Get<Keyboard>();
}

void UpdateSystem(ecs::Commands& cmd, ecs::Querier queryer,
                  ecs::Resources resources, ecs::Events&) {
    auto& mouse = resources.Get<Mouse>();
    auto& assets = resources.Get<AssetsManager>();

    if (resources.Has<Renderer>()) {
        auto& renderer = resources.Get<Renderer>();
        renderer.SetDrawColor(Color{0, 255, 0});
        renderer.DrawLine({100, 200}, {400, 500});
        renderer.SetDrawColor(Color{255, 255, 0});
        renderer.DrawRect(math::Rect{100, 200, 400, 500});
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
            .AddSystem(UpdateSystem)
            .AddSystem(InputHandle)
            .AddSystem(ShowDebugInfoSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(GameApp)
