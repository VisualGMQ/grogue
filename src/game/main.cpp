#include "app/app.hpp"

void StartupSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& renderer = resources.Get<Renderer>();
    cmd.SetResource<ImageHandle>(renderer.LoadImage("resources/img/role.png"));
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

class GameApp final : public App {
public:
    GameApp() {
        auto& world = GetWorld();
        world.AddStartupSystem(StartupSystem)
             .AddSystem(UpdateSystem)
             .AddSystem(InputHandle);
    }
};

int main(int argc, char** argv) {
    GameApp app;
    app.Run();
    return 0;
}
