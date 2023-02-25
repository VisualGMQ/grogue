#pragma once

#include "core/ecs.hpp"

void RenderSpriteSystem(ecs::Commands& cmd, ecs::Querier querier,
                        ecs::Resources resources, ecs::Events& events);