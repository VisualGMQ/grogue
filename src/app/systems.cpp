#include "app/systems.hpp"
#include "app/sprite.hpp"
#include "app/renderer.hpp"
#include "app/scene.hpp"

ecs::Entity FindRoot(ecs::Entity entity, ecs::Querier querier) {
    assert(querier.Has<Node>(entity));

    auto& node = querier.Get<Node>(entity);

    if (node.parent) {
        return FindRoot(node.parent.value(), querier);
    }
    return entity;
}

void PreorderVisit(ecs::Entity entity, ecs::Querier querier, std::function<void(ecs::Entity)> func) {
    func(entity);

    assert(querier.Has<Node>(entity));

    auto& node = querier.Get<Node>(entity);

    for (auto& child : node.children) {
        PreorderVisit(child, querier, func);
    }
}

void PostorderVisit(ecs::Entity entity, ecs::Querier querier, std::function<void(ecs::Entity)> func) {
    assert(querier.Has<Node>(entity));

    auto& node = querier.Get<Node>(entity);

    for (auto& child : node.children) {
        PreorderVisit(child, querier, func);
    }

    func(entity);
}

void renderOneSprite(ecs::Entity entity, ecs::Querier querier, Renderer& renderer) {
    SpriteBundle& bundle = querier.Get<SpriteBundle>(entity);
    if (querier.Has<Transform>(entity)) {
        auto& transform = querier.Get<Transform>(entity);
        renderer.DrawSprite(bundle, transform);
    } else if (querier.Has<NodeTransform>(entity)) {
        auto& transform = querier.Get<NodeTransform>(entity);
        renderer.DrawSprite(bundle, transform.globalTransform);
    } else {
        renderer.DrawSprite(bundle, Transform{});
    }
}

void RenderSpriteSystem(ecs::Commands& cmd, ecs::Querier querier,
                        ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();

    auto entities = querier.Query<SpriteBundle>();
    for (auto entity : entities) {
        renderOneSprite(entity, querier, renderer);
    }

    entities = querier.Query<Node>();
    for (auto entity : entities) {
        auto& node = querier.Get<Node>(entity);
        if (!node.parent) {
            PreorderVisit(entity, querier, [&](ecs::Entity entity) {
                renderOneSprite(entity, querier, renderer);
            });
        }
    }
}

void renderOneShape(ecs::Entity entity, ecs::Querier querier, Renderer& renderer) {
    Shape& shape = querier.Get<Shape>(entity);
    if (querier.Has<Transform>(entity)) {
        Transform& transform = querier.Get<Transform>(entity);
        renderer.DrawShape(shape, transform);
    } else if (querier.Has<NodeTransform>(entity)) {
        auto& transform = querier.Get<NodeTransform>(entity);
        renderer.DrawShape(shape, transform.globalTransform);
    } else {
        renderer.DrawShape(shape, Transform{});
    }
}

void RenderShapeSystem(ecs::Commands& cmd, ecs::Querier querier,
                       ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();

    auto entities = querier.Query<Shape>();
    for (auto entity : entities) {
        renderOneShape(entity, querier, renderer);
    }

    entities = querier.Query<Node>();
    for (auto entity : entities) {
        auto& node = querier.Get<Node>(entity);
        if (!node.parent) {
            PreorderVisit(entity, querier, [&](ecs::Entity entity) {
                renderOneShape(entity, querier, renderer);
            });
        }
    }
}

void UpdateTransformSystem(ecs::Commands& cmd, ecs::Querier querier,
                            ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();

    auto entities = querier.Query<SpriteBundle>();
    for (auto entity : entities) {
        auto& node = querier.Get<Node>(entity);

        if (!node.parent) {
            for (auto child : node.children) {
                PreorderVisit(child, querier, [&](ecs::Entity entity) {
                    auto& node = querier.Get<Node>(child);

                    if (!querier.Has<NodeTransform>(child) || !querier.Has<NodeTransform>(node.parent.value())) {
                        return;
                    }

                    auto& transform = querier.Get<NodeTransform>(child);
                    auto& parentTransform = querier.Get<NodeTransform>(node.parent.value());

                    transform.globalTransform.position = transform.localTransform.position + parentTransform.globalTransform.position;
                    transform.globalTransform.scale = transform.localTransform.scale * parentTransform.globalTransform.scale;
                    transform.globalTransform.rotation = transform.localTransform.rotation + parentTransform.globalTransform.rotation;
                });
            }
        }
    }
}
