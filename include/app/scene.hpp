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
