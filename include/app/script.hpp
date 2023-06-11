#pragma once

#include "app/app.hpp"
#include "app/manager.hpp"

//! @brief a component for running lua script
struct Script final {
    LuaScript lua;
    bool work = true;
    bool inited = false;

    static Script Create(LuaScript&& lua);
};

//! @brief resources for sharing variable between lua
struct LUA_BIND_RESOURCE LuaShareContext final {
    sol::table context;
};

//! @brief resources for sharing variable between lua
struct LUA_BIND LuaEventContext final {
    sol::table context;
};

void RunScriptSystem(ecs::Commands& cmd, ecs::Querier querier,
                    ecs::Resources resources, ecs::Events& events);

void HierarchyRunScriptSystem(std::optional<ecs::Entity>, ecs::Entity,
                           ecs::Commands&, ecs::Querier, ecs::Resources,
                           ecs::Events&);