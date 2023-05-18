#pragma once

#include "core/ecs.hpp"
#include "app/transform.hpp"

//! @brief use for hierarchy
struct Node {
    std::optional<ecs::Entity> parent;
    std::vector<ecs::Entity> children;
};

//! @brief use for transform in hierarchy
struct NodeTransform {
    Transform localTransform;
    Transform globalTransform;
};

ecs::Entity FindRoot(ecs::Entity entity, ecs::Querier querier);
void PreorderVisit(ecs::Entity entity, ecs::Querier querier, std::function<void(ecs::Entity)> func);
void PostorderVisit(ecs::Entity entity, ecs::Querier querier, std::function<void(ecs::Entity)> func);

class HierarchyBuilder final {
public:
    HierarchyBuilder(ecs::Commands& cmds, ecs::Entity parent): cmds_(cmds), parent_(parent) {}

    //! @brief set entity childrens
    //! @warning this will reset `NodeTransform` and `Node` component
    ecs::Entity SetChilds(const std::initializer_list<ecs::Entity>& entities) {
        cmds_.AddComponent<Node, NodeTransform>(parent_, Node{std::nullopt, entities}, NodeTransform{});
        for (auto entity : entities) {
            cmds_.AddComponent<Node, NodeTransform>(entity, Node{parent_, {}}, NodeTransform{});
        }
        return parent_;
    }

private:
    ecs::Commands& cmds_;
    ecs::Entity parent_;
};