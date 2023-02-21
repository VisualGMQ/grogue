#include "app/app.hpp"

void DetectMouseInputSystem(ecs::Commands&, ecs::Querier,
                            ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();
    auto& mouse = resources.Get<Mouse>();

    if (mouse.LeftBtn().IsPressing()) {
        renderer.SetDrawColor({255, 0, 0});
    }
    if (mouse.LeftBtn().IsReleasing()) {
        renderer.SetDrawColor({0, 255, 0});
    }
    if (mouse.LeftBtn().IsPressed()) {
        renderer.SetDrawColor({255, 255, 0});
        LOGT("pressed");
    }
    if (mouse.LeftBtn().IsReleased()) {
        renderer.SetDrawColor({0, 255, 255});
        LOGT("released");
    }
    constexpr int RectLen = 25;
    renderer.DrawRect({mouse.Position().x - RectLen, mouse.Position().y - RectLen, RectLen * 2, RectLen * 2});
}

void DetectKeyboardInputSystem(ecs::Commands&, ecs::Querier,
                               ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();
    auto& keyboard = resources.Get<Keyboard>();

    if (keyboard.Key(SDL_SCANCODE_W).IsPressing()) {
        renderer.SetDrawColor({255, 0, 0});
    }
    if (keyboard.Key(SDL_SCANCODE_W).IsReleasing()) {
        renderer.SetDrawColor({0, 255, 0});
    }
    if (keyboard.Key(SDL_SCANCODE_W).IsPressed()) {
        renderer.SetDrawColor({255, 255, 0});
        LOGT("pressed");
    }
    if (keyboard.Key(SDL_SCANCODE_W).IsReleased()) {
        renderer.SetDrawColor({0, 255, 255});
        LOGT("released");
    }
    constexpr int RectLen = 100;
    renderer.DrawRect({0, 0, RectLen, RectLen});
}

class InputTest final : public App {
public:
    InputTest() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
            .AddSystem(DetectMouseInputSystem)
            .AddSystem(DetectKeyboardInputSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(InputTest)
