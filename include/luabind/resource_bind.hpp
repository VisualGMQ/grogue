#pragma once

#include "core/pch.hpp"
#include "app/lua.hpp"
#include "app/app.hpp"

namespace lua_bind {

class LuaResources final {
public:
    LuaResources(ecs::Resources res): res_(res) {}
    ::Keyboard& GetKeyboard();
    ::Mouse& GetMouse();
    ::Time& GetTime();

private:
    ecs::Resources res_;
};

void BindLuaResources(::LuaScript& script);
void BindKeyboard(::LuaScript& script);
void BindMouse(::LuaScript& script);
void BindTime(::LuaScript& script);

}