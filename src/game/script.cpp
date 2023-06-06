#include "game/script.hpp"
#include "luabind/luabind.hpp"

void RunOneScript(Script& script, ecs::Resources res) {
    if (script.work) {
        lua_bind::BindLua(script.lua);
        lua_bind::LuaResources luaRes(res);
        auto func = script.lua.lua["run"];
        if (func.get_type() != sol::type::function) {
            std::function<void(lua_bind::LuaResources)> f = func;
            f(luaRes);
        }
    }
}

void RunScriptSystem(ecs::Commands& cmd, ecs::Querier querier,
                    ecs::Resources res, ecs::Events& events) {
    auto entities = querier.Query<Script>();
    for (auto entity : entities) {
        auto& script = querier.Get<Script>(entity);
        RunOneScript(script, res);
    }
}

void HierarchyRunScriptSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                           ecs::Commands&, ecs::Querier querier, ecs::Resources res,
                           ecs::Events&) {
    if (!querier.Has<Script>(entity)) return;

    auto& script = querier.Get<Script>(entity);
    RunOneScript(script, res);
}