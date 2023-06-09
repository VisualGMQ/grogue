#include "luabind/commands_bind.hpp"

namespace lua_bind {

void BindCommandsWrapper(LuaScript& script) {
    sol::usertype<CommandsWrapper> cmd = script.lua.new_usertype<CommandsWrapper>("Commands");
    cmd["add_component"] = sol::overload(
        static_cast<void(CommandsWrapper::*)(ecs::Entity, Node)>(&CommandsWrapper::Add),
        static_cast<void(CommandsWrapper::*)(ecs::Entity, SpriteBundle)>(&CommandsWrapper::Add),
        static_cast<void(CommandsWrapper::*)(ecs::Entity, RectTransform)>(&CommandsWrapper::Add),
        static_cast<void(CommandsWrapper::*)(ecs::Entity, NodeTransform)>(&CommandsWrapper::Add),
        static_cast<void(CommandsWrapper::*)(ecs::Entity, Panel)>(&CommandsWrapper::Add),
        static_cast<void(CommandsWrapper::*)(ecs::Entity, Transform)>(&CommandsWrapper::Add));
    cmd["spawn"] = &CommandsWrapper::Spawn;
    cmd["destroy_entity"] = &CommandsWrapper::DestroyEntity;
}

void BindEventsWrapper(LuaScript& script) {
    sol::usertype<EventsWrapper> cmd = script.lua.new_usertype<EventsWrapper>("Events");
}

}