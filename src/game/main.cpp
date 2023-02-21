#include "app/app.hpp"

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

        auto& handle = resources.Get<ImageHandle>();

        Transform transform;
        transform.position = mouse.Position();

        SpriteBundle sprite{Sprite::Default(), handle, transform};

        renderer.DrawSprite(sprite);
    }
}

void ShowDebugInfoSystem(ecs::Commands&, ecs::Querier, ecs::Resources resources,
                         ecs::Events&) {
    auto& context = resources.Get<Context>();
    auto& renderer = resources.Get<Renderer>();
    auto& timer = resources.Get<Timer>();
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
            .AddSystem(UpdateSystem)
            .AddSystem(InputHandle)
            .AddSystem(ShowDebugInfoSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(GameApp)
