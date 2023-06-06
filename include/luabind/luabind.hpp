#pragma once

#include "core/pch.hpp"
#include "app/lua.hpp"
#include "luabind/component_bind.hpp"
#include "luabind/resource_bind.hpp"

namespace lua_bind {

void BindLua(LuaScript&);

}