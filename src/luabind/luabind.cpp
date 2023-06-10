#include "luabind/luabind.hpp"
#include "luabind/component_bind.hpp"
#include "luabind/resource_bind.hpp"
#include "game/types.hpp"

namespace lua_bind {

void bindGameRelated(LuaScript& script) {
    sol::usertype<BackpackUIPanel> panel = script.lua.new_usertype<BackpackUIPanel>("BackpackUIPanel");

    sol::usertype<Player> player = script.lua.new_usertype<Player>("BackpackUIPanel");

    sol::usertype<Backpack> backpack = script.lua.new_usertype<Backpack>("Backpack");
    backpack["items"] = &Backpack::items;

    sol::usertype<Item> item = script.lua.new_usertype<Item>("Item");
    item["amount"] = &Item::amount;
    item["nameID"] = &Item::nameID;
}

void BindLua(LuaScript& script) {
    BindCommandsWrapper(script);
    BindEventsWrapper(script);
    BindResourcesWrapper(script);
    BindQuerierWrapper(script);
    BindResources(script);
    BindEventsWrapper(script);
    BindEvents(script);
    BindSimpleTypes(script);
    BindComponents(script);

    bindGameRelated(script);
}

}