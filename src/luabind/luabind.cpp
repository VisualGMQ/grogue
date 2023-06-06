#include "luabind/luabind.hpp"

namespace lua_bind {

void BindLua(LuaScript& script) {
    BindLuaResources(script);
    BindMouse(script);
    BindKeyboard(script);
    BindTime(script);
}

}