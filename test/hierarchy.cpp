#include "app/app.hpp"
#include "app/scene.hpp"
#include "test_helper.hpp"

void CreateEntities(ecs::Commands& cmd, ecs::Resources resources) {
    auto center = math::Vector2(0, 0);
    ecs::Entity root = cmd.SpawnAndReturn<Shape, NodeTransform>(Shape::CreateRect(center, math::Vector2(200, 200), Color::Red), NodeTransform{});
    HierarchyBuilder(cmd, root).SetChilds(
        {
            cmd.SpawnAndReturn<Shape, NodeTransform>(Shape::CreateRect(center, math::Vector2(150, 150), Color::Green), NodeTransform{})
        }
    );

    cmd.SetResource<ecs::Entity>(std::move(root));
}

void UpdateRoot(ecs::Commands& cmd, ecs::Querier querier, ecs::Resources resources, ecs::Events&) {
    auto& root = resources.Get<ecs::Entity>();
    auto& transform = querier.Get<NodeTransform>(root);
    transform.localTransform.position.x += 2;
    transform.localTransform.position.y += 1;
    transform.localTransform.rotation += 1;
}

class Test : public App {
public:
    Test() {
        auto& world = GetWorld();
        world
            .AddPlugins<DefaultPlugins>()
            .AddStartupSystem(CreateEntities)
            .AddSystem(UpdateRoot)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)