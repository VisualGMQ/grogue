#include "luabind/luabind.hpp"

namespace lua_bind {

void BindLua(LuaScript& script) {
    BindLuaResources(script);
    BindResources(script);
    BindSimpleTypes(script);
    BindComponents(script);
    BindLuaQuerier(script);
}

}