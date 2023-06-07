#include "game/script.hpp"
#include "luabind/luabind.hpp"

void runOneScript(ecs::Entity entity, ecs::Commands& cmds, Script& script, ecs::Resources res, ecs::Querier querier) {
    if (script.work) {
        lua_bind::BindLua(script.lua);
        lua_bind::LuaResources luaRes(res);
        lua_bind::LuaCommands luaCmds(cmds);
        lua_bind::LuaQuerier luaQuerier(querier);
        auto func = script.lua.lua["Run"];
        if (func.get_type() == sol::type::function) {
            std::function<void(ecs::Entity, lua_bind::LuaCommands&,
                               lua_bind::LuaQuerier, lua_bind::LuaResources)>
                f = func;
            f(entity, luaCmds, luaQuerier, res);
        }
    }
}

void RunScriptSystem(ecs::Commands& cmds, ecs::Querier querier,
                    ecs::Resources res, ecs::Events& events) {
    auto entities = querier.Query<Script>();
    for (auto entity : entities) {
        auto& script = querier.Get<Script>(entity);
        runOneScript(entity, cmds, script, res, querier);
    }
}

void HierarchyRunScriptSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                           ecs::Commands& cmds, ecs::Querier querier, ecs::Resources res,
                           ecs::Events&) {
    if (!querier.Has<Script>(entity)) return;

    auto& script = querier.Get<Script>(entity);
    runOneScript(entity, cmds, script, res, querier);
}