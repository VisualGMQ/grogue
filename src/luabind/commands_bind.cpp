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
    sol::usertype<EventsWrapper> events = script.lua.new_usertype<EventsWrapper>("Events");
    events["read_event_context"] = &EventsWrapper::ReadEventContext;
    events["write_event_context"] = &EventsWrapper::WriteEventContext;
}

template <typename T>
void bindEventReader(::LuaScript& script, const std::string& name) {
    using ReaderType = ecs::EventReader<T>;
    sol::usertype<ReaderType> event = script.lua.new_usertype<ReaderType>(name + "Reader");
    event["has"] = &ReaderType::Has;
    event["read"] = &ReaderType::Read;
    event["clear"] = &ReaderType::Clear;
}

template <typename T>
void bindEventWriter(::LuaScript& script, const std::string& name) {
    using WriterType = ecs::EventWriter<T>;
    sol::usertype<WriterType> event = script.lua.new_usertype<WriterType>(name + "Writer");
    event["write"] = &WriterType::Write;
    event["write_immediatly"] = sol::overload(
        static_cast<void(WriterType::*)(const T&)>(&WriterType::WriteImmediate));
}

template <typename T>
void bindEvent(::LuaScript& script, const std::string& name) {
    bindEventReader<T>(script, name);
    bindEventWriter<T>(script, name);
}

void BindEvents(LuaScript& script) {
    bindEvent<::LuaEventContext>(script, "EventContext");
}

}