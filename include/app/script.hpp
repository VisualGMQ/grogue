#pragma once

#include "app/app.hpp"
#include "app/manager.hpp"

//! @brief a component for running lua script
struct Script final {
    std::shared_ptr<LuaScript> lua;
    bool work = true;
    bool inited = false;

    static Script Create(LuaScript&& lua);
};

//! @brief resources for sharing variable between lua
struct LUA_BIND_RESOURCE LuaShareContext final {
    sol::object context;
};

//! @brief resources for sharing variable between lua
struct LUA_BIND LuaEventContext final {
    sol::object context;
};

void RunScriptSystem(ecs::Commands& cmd, ecs::Querier querier,
                    ecs::Resources resources, ecs::Events& events);

void HierarchyRunScriptSystem(std::optional<ecs::Entity>, ecs::Entity,
                           ecs::Commands&, ecs::Querier, ecs::Resources,
                           ecs::Events&);