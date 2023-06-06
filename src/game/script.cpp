#include "game/script.hpp"
#include "luabind/luabind.hpp"

void runOneScript(ecs::Entity entity, Script& script, ecs::Resources res, ecs::Querier querier) {
    if (script.work) {
        lua_bind::BindLua(script.lua);
        lua_bind::LuaResources luaRes(res);
        lua_bind::LuaQuerier luaQuerier(querier);
        auto func = script.lua.lua["Run"];
        if (func.get_type() == sol::type::function) {
            std::function<void(ecs::Entity, lua_bind::LuaResources, lua_bind::LuaQuerier)> f = func;
            f(entity, luaRes, luaQuerier);
        }
    }
}

void RunScriptSystem(ecs::Commands& cmd, ecs::Querier querier,
                    ecs::Resources res, ecs::Events& events) {
    auto entities = querier.Query<Script>();
    for (auto entity : entities) {
        auto& script = querier.Get<Script>(entity);
        runOneScript(entity, script, res, querier);
    }
}

void HierarchyRunScriptSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                           ecs::Commands&, ecs::Querier querier, ecs::Resources res,
                           ecs::Events&) {
    if (!querier.Has<Script>(entity)) return;

    auto& script = querier.Get<Script>(entity);
    runOneScript(entity, script, res, querier);
}