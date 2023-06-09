#include "luabind/luabind.hpp"
#include "luabind/component_bind.hpp"
#include "luabind/resource_bind.hpp"

namespace lua_bind {

void BindLua(LuaScript& script) {
    BindCommandsWrapper(script);
    BindEventsWrapper(script);
    BindResourcesWrapper(script);
    BindQuerierWrapper(script);
    BindResources(script);
    BindSimpleTypes(script);
    BindComponents(script);
}

}