#pragma once

#include "app/app.hpp"
#include "app/manager.hpp"

//! @brief a component for running lua script
struct Script final {
    std::shared_ptr<LuaScript> lua;
    bool work = true;
    bool inited = false;

    static Script Create(LuaManager& mgr, const std::string& filename);
};

void RunScriptSystem(ecs::Commands& cmd, ecs::Querier querier,
                    ecs::Resources resources, ecs::Events& events);

void HierarchyRunScriptSystem(std::optional<ecs::Entity>, ecs::Entity,
                           ecs::Commands&, ecs::Querier, ecs::Resources,
                           ecs::Events&);