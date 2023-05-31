#include "app/systems.hpp"
#include "app/renderer.hpp"
#include "app/scene.hpp"
#include "app/sprite.hpp"
#include "app/ui.hpp"

void renderOneSprite(ecs::Entity entity, ecs::Querier querier,
                     Renderer& renderer) {
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

    auto entities =
        querier.Query<ecs::With<SpriteBundle, ecs::Without<Node>>>();
    for (auto entity : entities) {
        renderOneSprite(entity, querier, renderer);
    }
}

void HierarchyRenderSpriteSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                                 ecs::Commands&, ecs::Querier querier,
                                 ecs::Resources resources,
                                 ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();

    if (querier.Has<SpriteBundle>(entity)) {
        renderOneSprite(entity, querier, renderer);
    }
}

void renderOneShape(ecs::Entity entity, ecs::Querier querier,
                    Renderer& renderer) {
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

    auto entities = querier.Query<ecs::With<Shape, ecs::Without<Node>>>();
    for (auto entity : entities) {
        renderOneShape(entity, querier, renderer);
    }
}

void HierarchyRenderShapeSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                                ecs::Commands& cmd, ecs::Querier querier,
                                ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();

    if (querier.Has<Shape>(entity)) {
        renderOneShape(entity, querier, renderer);
    }
}

void updateOneTransform(ecs::Entity entity, ecs::Querier querier) {

    auto& node = querier.Get<Node>(entity);
    auto& transform = querier.Get<NodeTransform>(entity);

    // FIXME: low performance, will query parent NodeTransform twice(when visit
    // parent, query entity's NodeTransform, then visit parent's child, query
    // node.parent NodeTransform) Solution: pass parent NodeTransform to this
    // function to void query
    if (node.parent && querier.Has<NodeTransform>(node.parent.value())) {
        auto& parent = querier.Get<NodeTransform>(node.parent.value());

        transform.globalTransform.position =
            transform.localTransform.position + parent.globalTransform.position;
        transform.globalTransform.scale =
            transform.localTransform.scale * parent.globalTransform.scale;
        transform.globalTransform.rotation =
            transform.localTransform.rotation + parent.globalTransform.rotation;
    } else if (!node.parent) {
        transform.globalTransform = transform.localTransform;
    }
}

void UpdateTransformSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                           ecs::Commands&, ecs::Querier querier, ecs::Resources,
                           ecs::Events&) {
    if (querier.Has<NodeTransform>(entity)) {
        updateOneTransform(entity, querier);
    }
}

void updateButtonUI(std::optional<ecs::Entity> parent, ecs::Entity entity,
                 ecs::Commands& cmds, ecs::Querier querier,
                 ecs::Resources res,
                 ecs::Events& events) {
    if (!querier.Has<ui::Button>(entity)) {
        return;
    }

    auto& renderer = res.Get<Renderer>();
    ui::Widget& widget = querier.Get<ui::Widget>(entity);
    ui::Button& button = querier.Get<ui::Button>(entity);
    button.system(parent, entity, cmds, querier, res, events);
}

void updateOneUI(std::optional<ecs::Entity> parent, ecs::Entity entity,
                 ecs::Commands& cmds, ecs::Querier querier,
                 ecs::Resources res,
                 ecs::Events& events) {
    updateButtonUI(parent, entity, cmds, querier, res, events);
}

void UpdateUISystem(ecs::Commands& cmds, ecs::Querier querier,
                    ecs::Resources res, ecs::Events& events) {
    auto entities = querier.Query<ecs::With<ui::Widget, ecs::Without<Node>>>();
    for (auto entity : entities) {
        updateOneUI(std::nullopt, entity, cmds, querier, res, events);
    }
}

void HierarchyUpdateUISystem(std::optional<ecs::Entity>, ecs::Entity entity,
                             ecs::Commands& cmds, ecs::Querier querier,
                             ecs::Resources res, ecs::Events& events) {
    if (querier.Has<ui::Widget>(entity)) {
        updateOneUI(std::nullopt, entity, cmds, querier, res, events);
    }
}
