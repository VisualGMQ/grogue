#include "script/script.hpp"
#include "script/luabind.hpp"
#include "ecs_wrapper.hpp"

extern void BindLua(sol::state&);

Script Script::Create(LuaManager& mgr, const std::string& filename) {
    auto lua = mgr.CreateSolitary();
    lua.lua.do_string("package.path = package.path .. \";./resources/script/ecs.lua\"");
    lua.lua.do_string("Script = {}");
    BindLua(lua.lua);
    lua.lua.do_file(filename);
    return std::move(Script{std::make_shared<LuaScript>(std::move(lua))});
}

void runOneScript(ecs::Entity entity, ecs::Commands& cmds, Script& script, ecs::Resources res, ecs::Querier querier, ecs::Events& events) {
    if (script.work) {
        sol::table scriptTable = script.lua->lua["Script"];
        if (!scriptTable.valid()) {
            LOGW("[LuaScript Error]: your script don't has Script table");
            return;
        }

        ::ResourcesWrapper luaRes(res);
        ::CommandsWrapper luaCmds(cmds);
        ::QuerierWrapper luaQuerier(querier);
        ::EventsWrapper luaEvents(events);
        if (!script.inited) {
            script.inited = true;
            auto func = scriptTable["Startup"];
            if (func.valid()) {
                sol::protected_function f = func;
                sol::protected_function_result result = f(entity, std::ref(luaCmds), std::ref(luaRes));
                if (!result.valid()) {
                    sol::error err = result;
                    LOGE("[Lua Error]: ", err.what());
                    script.lua->lua.do_string("debug.traceback()");
                }
            }
        }
        auto func = scriptTable["Run"];
        if (func.get_type() == sol::type::function) {
            sol::protected_function f = func;
            sol::protected_function_result result = f(entity, std::ref(luaCmds), std::ref(luaQuerier), std::ref(luaRes), std::ref(luaEvents));
            if (!result.valid()) {
                    sol::error err = result;
                    LOGE("[Lua Error]: ", err.what());
                    script.lua->lua.do_string("debug.traceback()");
            }
        }
    }
}

void RunScriptSystem(ecs::Commands& cmds, ecs::Querier querier,
                    ecs::Resources res, ecs::Events& events) {
    auto entities = querier.Query<ecs::With<Script, ecs::Without<Node>>>();
    for (auto entity : entities) {
        auto& script = querier.Get<Script>(entity);
        runOneScript(entity, cmds, script, res, querier, events);
    }
}

void HierarchyRunScriptSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                           ecs::Commands& cmds, ecs::Querier querier, ecs::Resources res,
                           ecs::Events& events) {
    if (!querier.Has<Script>(entity)) return;

    auto& script = querier.Get<Script>(entity);
    runOneScript(entity, cmds, script, res, querier, events);
}
