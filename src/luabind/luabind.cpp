#include "luabind/luabind.hpp"

namespace lua_bind {

void bindLuaCommands(LuaScript& script) {
    sol::usertype<LuaCommands> cmd = script.lua.new_usertype<LuaCommands>("Commands");
    cmd["add_component"] = sol::overload(
        static_cast<void(LuaCommands::*)(ecs::Entity, Node)>(&LuaCommands::Add),
        static_cast<void(LuaCommands::*)(ecs::Entity, SpriteBundle)>(&LuaCommands::Add),
        static_cast<void(LuaCommands::*)(ecs::Entity, RectTransform)>(&LuaCommands::Add),
        static_cast<void(LuaCommands::*)(ecs::Entity, NodeTransform)>(&LuaCommands::Add),
        static_cast<void(LuaCommands::*)(ecs::Entity, Panel)>(&LuaCommands::Add),
        static_cast<void(LuaCommands::*)(ecs::Entity, Transform)>(&LuaCommands::Add));
    cmd["spawn"] = &LuaCommands::Spawn;
    cmd["destroy_entity"] = &LuaCommands::DestroyEntity;
}

void BindLua(LuaScript& script) {
    bindLuaCommands(script);
    BindLuaResources(script);
    BindResources(script);
    BindSimpleTypes(script);
    BindComponents(script);
    BindLuaQuerier(script);
}

}