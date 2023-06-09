#include "app/script.hpp"
#include "luabind/luabind.hpp"

Script Script::Create(LuaScript&& lua) {
    lua_bind::BindLua(lua);
    return Script{std::move(lua)};
}

void runOneScript(ecs::Entity entity, ecs::Commands& cmds, Script& script, ecs::Resources res, ecs::Querier querier, ecs::Events& events) {
    if (script.work) {
        lua_bind::ResourcesWrapper luaRes(res);
        lua_bind::CommandsWrapper luaCmds(cmds);
        lua_bind::QuerierWrapper luaQuerier(querier);
        lua_bind::EventsWrapper luaEvents(events);
        if (!script.inited) {
            script.inited = true;
            auto func = script.lua.lua["Startup"];
            if (func.valid()) {
                static_cast<std::function<void(ecs::Entity, lua_bind::CommandsWrapper&, lua_bind::ResourcesWrapper)>>(func)(entity, luaCmds, luaRes);
            }
        }
        auto func = script.lua.lua["Run"];
        if (func.get_type() == sol::type::function) {
            static_cast<std::function<void(ecs::Entity, lua_bind::CommandsWrapper,
                               lua_bind::QuerierWrapper, lua_bind::ResourcesWrapper, lua_bind::EventsWrapper)>>(func)(entity, luaCmds, luaQuerier, luaRes, luaEvents);
        }
    }
}

void RunScriptSystem(ecs::Commands& cmds, ecs::Querier querier,
                    ecs::Resources res, ecs::Events& events) {
    auto entities = querier.Query<Script>();
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
