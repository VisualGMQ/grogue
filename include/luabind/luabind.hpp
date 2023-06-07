#pragma once

#include "core/pch.hpp"
#include "app/lua.hpp"
#include "luabind/component_bind.hpp"
#include "luabind/resource_bind.hpp"

namespace lua_bind {

void BindLua(LuaScript&);

using ui::Panel;

// FIXME: elem is lvalue, but `AddComponent` need rvalue, and sol bind lua can't
// use rvalue. So we use lvalue here temporary
#define IMPL_ADD_FUNC(type) \
void Add(ecs::Entity entity, type elem) { \
    cmds_.AddComponent<type>(entity, std::move(elem)); \
}

struct LuaCommands final {
public:
    LuaCommands(ecs::Commands& cmds): cmds_(cmds) {}

    ecs::Entity Spawn() { return cmds_.SpawnAndReturn(); }
    void DestroyEntity(ecs::Entity entity) { cmds_.DestroyEntity(entity); }

    IMPL_ADD_FUNC(Node)
    IMPL_ADD_FUNC(SpriteBundle)
    IMPL_ADD_FUNC(Panel)
    IMPL_ADD_FUNC(Transform)
    IMPL_ADD_FUNC(NodeTransform)
    IMPL_ADD_FUNC(RectTransform)

private:
    ecs::Commands& cmds_;
};

}