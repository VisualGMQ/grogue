#include "app/app.hpp"
#include "test_helper.hpp"

void CreateEntities(ecs::Commands& cmd, ecs::Resources resources) {
    cmd.Spawn<Shape, Transform>(Shape::CreateCircle(math::Vector2::Zero, 50.0, 20, Color::Green),
                                Transform{math::Vector2(100.0, 100.0)});

}

class Test : public App {
public:
    Test() {
        auto& world = GetWorld();
        world
            .AddPlugins<DefaultPlugins>()
            .AddStartupSystem(CreateEntities)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)