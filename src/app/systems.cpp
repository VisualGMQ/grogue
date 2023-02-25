#include "app/systems.hpp"
#include "app/sprite.hpp"
#include "app/renderer.hpp"
#include "app/scene.hpp"

void PreorderVisit(const Node* parent, const Node& node, std::function<void(const Node*, const Node&)> func) {
    func(parent, node);
    for (auto& child : node.children) {
        PreorderVisit(&node, child, func);
    }
}

void RenderSpriteSystem(ecs::Commands& cmd, ecs::Querier querier,
                        ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();
    auto& scene = resources.Get<Scene>();
    auto& root = scene.root;

    PreorderVisit(nullptr, root, [&](const Node* parent, const Node& node){
        if (!node.entity.has_value()) return;

        auto entity = node.entity.value();
        if (!querier.Has<SpriteBundle>(entity) || !querier.Has<NodeTransform>(entity)) {
            return;
        }

        auto& spriteBundle = querier.Get<SpriteBundle>(entity);
        auto& transform = querier.Get<NodeTransform>(entity);
        renderer.DrawSprite(spriteBundle, transform.globalTransform);
    });
}

void UpdateTransformSystem(ecs::Commands& cmd, ecs::Querier querier,
                            ecs::Resources resources, ecs::Events& events) {
    auto& scene = resources.Get<Scene>();
    auto& root = scene.root;

    PreorderVisit(nullptr, root, [&](const Node* parent, const Node& node) {
        if (!node.entity.has_value() ||
            !querier.Has<NodeTransform>(node.entity.value())) {
            return;
        }

        auto entity = node.entity.value();

        auto& transform = querier.Get<NodeTransform>(entity);

        if (!parent || !querier.Has<NodeTransform>(parent->entity.value())) {
            transform.globalTransform = transform.localTransform;
            return;
        }

        auto& parentTransform = querier.Get<NodeTransform>(parent->entity.value());

        transform.globalTransform.position = transform.localTransform.position + parentTransform.globalTransform.position;
        transform.globalTransform.scale = transform.localTransform.scale * parentTransform.globalTransform.scale;
        transform.globalTransform.rotation = transform.localTransform.rotation + parentTransform.globalTransform.rotation;
    });
}

void RenderDebugSystem(ecs::Commands& cmd, ecs::Querier querier,
                       ecs::Resources resources, ecs::Events& events) {
}