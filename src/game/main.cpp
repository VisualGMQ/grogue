#include "app/app.hpp"

struct Context {
    FontHandle font;
};

void LoadResourceSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& renderer = resources.Get<Renderer>();
    auto& fontManager = resources.Get<FontManager>();
    cmd.SetResource<ImageHandle>(renderer.LoadImage("resources/img/role.png"))
       .SetResource<Context>(Context{fontManager.Load("resources/font/SimHei.ttf", 14)});
}

void InputHandle(ecs::Commands& cmd, ecs::Queryer queryer,
                 ecs::Resources resources, ecs::Events& events) {
    auto& keyboard = resources.Get<Keyboard>();
}

void UpdateSystem(ecs::Commands& cmd, ecs::Queryer queryer,
                  ecs::Resources resources, ecs::Events&) {
    auto& mouse = resources.Get<Mouse>();

    if (resources.Has<Renderer>()) {
        auto& renderer = resources.Get<Renderer>();
        renderer.SetDrawColor(Color{0, 255, 0});
        renderer.DrawLine({100, 200}, {400, 500});
        renderer.SetDrawColor(Color{255, 255, 0});
        renderer.DrawRect(math::Rect{100, 200, 400, 500});

        auto& handle = resources.Get<ImageHandle>();
        auto img = renderer.GetImage(handle);

        renderer.DrawImage(
            *img,
            {0, 0, static_cast<float>(img->W()), static_cast<float>(img->H())},
            {mouse.Position().x, mouse.Position().y,
             static_cast<float>(img->W()), static_cast<float>(img->H())});
    }
}

void ShowDebugInfoSystem(ecs::Commands&, ecs::Queryer,
                         ecs::Resources resources, ecs::Events&) {
    auto& context = resources.Get<Context>();
    auto& renderer = resources.Get<Renderer>();
    auto& timer = resources.Get<Timer>();

    renderer.DrawText(context.font, "fps: " + std::to_string(static_cast<uint32_t>(1000.0 / timer.Elapse())), {0, 0}, {255, 255, 255});
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
