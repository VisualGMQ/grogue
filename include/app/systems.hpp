#pragma once

#include "core/ecs.hpp"

void RenderSpriteSystem(ecs::Commands&, ecs::Querier, ecs::Resources,
                        ecs::Events&);
void HierarchyRenderSpriteSystem(std::optional<ecs::Entity>, ecs::Entity,
                                 ecs::Commands&, ecs::Querier, ecs::Resources,
                                 ecs::Events&);

void RenderShapeSystem(ecs::Commands&, ecs::Querier, ecs::Resources,
                       ecs::Events&);
void HierarchyRenderShapeSystem(std::optional<ecs::Entity>, ecs::Entity,
                                ecs::Commands&, ecs::Querier,
                                ecs::Resources, ecs::Events&);

void UpdateNodeTransformSystem(std::optional<ecs::Entity>, ecs::Entity,
                           ecs::Commands&, ecs::Querier, ecs::Resources,
                           ecs::Events&);

void UpdateRectTransformSystem(std::optional<ecs::Entity>, ecs::Entity,
                           ecs::Commands&, ecs::Querier, ecs::Resources,
                           ecs::Events&);

struct DebugConfig {
    bool showAnchor = false;
};