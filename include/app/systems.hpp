#pragma once

#include "core/ecs.hpp"

void RenderSpriteSystem(ecs::Commands& cmd, ecs::Querier querier,
                        ecs::Resources resources, ecs::Events& events);

void UpdateTransformSystem(ecs::Commands& cmd, ecs::Querier querier,
                            ecs::Resources resources, ecs::Events& events);

void RenderDebugSystem(ecs::Commands& cmd, ecs::Querier querier,
                        ecs::Resources resources, ecs::Events& events);

struct DebugConfig {
    bool showAnchor = false;
};