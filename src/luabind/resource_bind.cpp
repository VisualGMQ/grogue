#include "luabind/resource_bind.hpp"
#include "game/config.hpp"

namespace lua_bind {

::Keyboard& LuaResources::GetKeyboard() {
    return res_.Get<::Keyboard>(); 
}

::Mouse& LuaResources::GetMouse() {
    return res_.Get<::Mouse>();
}

::Time& LuaResources::GetTime() {
    return res_.Get<::Time>();
}

void BindLuaResources(::LuaScript& script) {
    sol::usertype<LuaResources> resources = script.lua.new_usertype<LuaResources>("Resources");
    resources["get_keyboard"] = &LuaResources::GetKeyboard;
    resources["get_mouse"] = &LuaResources::GetMouse;
    resources["get_time"] = &LuaResources::GetTime;
}

void BindKeyboard(::LuaScript& script) {
    sol::usertype<::Keyboard> keyboard = script.lua.new_usertype<::Keyboard>("Keyboard");
    keyboard["key"] = &Keyboard::Key;
}

void BindMouse(::LuaScript& script) {
    sol::usertype<::Mouse> mouse = script.lua.new_usertype<::Mouse>("Mouse");
    mouse["Position"] = &Mouse::Position;
    mouse["Offset"] = &Mouse::Offset;
    mouse["LeftBtn"] = &Mouse::LeftBtn;
    mouse["RightBtn"] = &Mouse::RightBtn;
    mouse["MiddleBtn"] = &Mouse::MiddleBtn;
}

void BindTime(::LuaScript& script) {
    sol::usertype<::Time> time = script.lua.new_usertype<::Time>("Time");
    time["SetFPS"] = &Time::SetFPS;
    time["GetFPS"] = &Time::GetFPS;
    time["Elapse"] = &Time::Elapse;
}

}