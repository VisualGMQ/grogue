#pragma once

#include "core/ecs.hpp"
#include "core/luabind_tag.hpp"

void ResetRenderStateSystem(ecs::Commands& cmd, ecs::Querier querier,
                        ecs::Resources resources, ecs::Events& events);

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

void ToggleDebugMode(std::optional<ecs::Entity>, ecs::Entity,
                    ecs::Commands&, ecs::Querier, ecs::Resources,
                    ecs::Events&);

struct [[refl, luabind(comp)]] DebugConfig {
    bool showAnchor = false;
    bool showPhysicDebugInfo = false;
};