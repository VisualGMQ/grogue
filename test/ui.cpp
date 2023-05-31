#include "app/app.hpp"
#include "test_helper.hpp"

void CreateUISystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& fontMgr = resources.Get<AssetsManager>().Font();
    auto font = fontMgr.Load(TestHelper::Instance().GetResourcePath() + "PoppkornRegular.ttf", 20);
    cmd.Spawn<ui::Widget, ui::Button, Transform>(
        ui::Widget::CreateDefault(math::Vector2(200.0, 100.0)),
        ui::Button::CreateDefault("button", font, Color::Black),
        Transform::FromPosition(math::Vector2(100.0, 200.0)));
}

class Test : public App {
public:
    Test() {
        auto& world = GetWorld();
        world
            .AddPlugins<DefaultPlugins>()
            .AddStartupSystem(CreateUISystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)