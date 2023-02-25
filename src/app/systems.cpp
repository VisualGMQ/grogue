#include "app/systems.hpp"
#include "app/sprite.hpp"
#include "app/renderer.hpp"
#include "app/scene.hpp"

void PreorderVisit(const Node& node, std::function<void(const Node& node)> func) {
    func(node);
    for (auto& child : node.children) {
        PreorderVisit(child, func);
    }
}

void RenderSpriteSystem(ecs::Commands& cmd, ecs::Querier querier,
                        ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();
    auto& scene = resources.Get<Scene>();
    auto& root = scene.root;

    PreorderVisit(root, [&](const Node& node){
        if (!node.entity.has_value()) return;

        auto& spriteBundle = querier.Get<SpriteBundle>(node.entity.value());
        renderer.DrawSprite(spriteBundle);
    });
}