#pragma once

#include "app/app.hpp"

//! @brief a component for running lua script
struct Script final {
    LuaScript lua;
    bool work = true;
};

void RunScriptSystem(ecs::Commands& cmd, ecs::Querier querier,
                    ecs::Resources resources, ecs::Events& events);

void HierarchyRunScriptSystem(std::optional<ecs::Entity>, ecs::Entity,
                           ecs::Commands&, ecs::Querier, ecs::Resources,
                           ecs::Events&);