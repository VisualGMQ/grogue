#include "app/app.hpp"
#include "test_helper.hpp"

void CreateUISystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& fontMgr = resources.Get<AssetsManager>().Font();
    auto font = fontMgr.Load(TestHelper::Instance().GetResourcePath() + "PoppkornRegular.ttf", 20);
    cmd.Spawn<Node, ui::RectTransform, ui::Label>(
        Node{},
        ui::RectTransform{NodeTransform{Transform::FromPosition({100, 200})}, math::Vector2(100, 25)},
        ui::Label{ui::Text{font, "this is label", {Color::Black, Color::White, Color::Green}}}
    );
    cmd.Spawn<Node, ui::RectTransform, ui::Button, ui::Text, ui::Interaction>(
        Node{},
        ui::RectTransform{NodeTransform{Transform::FromPosition({400, 200})}, math::Vector2(100, 50)},
        ui::Button{ui::ColorBundle{Color::White, Color::Green, Color::Blue}, ui::ColorBundle{Color::Black, Color::Black, Color::Black}},
        ui::Text{font, "button", ui::ColorBundle{Color::Black, Color::Black, Color::Black}, {}},
        ui::Interaction{[](ecs::Entity, ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&) {
            LOGT("button click");
        }}
    );
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