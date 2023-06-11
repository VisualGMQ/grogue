#pragma once

#include "core/ecs.hpp"
#include "app/transform.hpp"

//! @brief use for transform in hierarchy
struct LUA_BIND_COMPONENT NodeTransform {
    Transform localTransform;
    Transform globalTransform;
};

class LUA_BIND HierarchyBuilder final {
public:
    HierarchyBuilder(ecs::Commands& cmds, ecs::Entity parent): cmds_(cmds), parent_(parent) {}

    //! @brief set entity childrens
    ecs::Entity LUA_NOBIND SetChilds(const std::initializer_list<ecs::Entity>& entities) {
        cmds_.AddComponent<Node>(parent_, Node{std::nullopt, entities});
        for (auto entity : entities) {
            cmds_.AddComponent<Node>(entity, Node{parent_, {}});
        }
        return parent_;
    }

    //! @brief set entity childrens
    ecs::Entity SetChilds(const std::vector<ecs::Entity>& entities) {
        cmds_.AddComponent<Node>(parent_, Node{std::nullopt, entities});
        for (auto entity : entities) {
            cmds_.AddComponent<Node>(entity, Node{parent_, {}});
        }
        return parent_;
    }

private:
    ecs::Commands& cmds_;
    ecs::Entity parent_;
};