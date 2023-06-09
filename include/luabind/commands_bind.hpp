#pragma once

#include "app/ui.hpp"
#include "core/pch.hpp"

namespace lua_bind {

using ui::Panel;
using ui::RectTransform;

// FIXME: elem is lvalue, but `AddComponent` need rvalue, and sol bind lua can't
// use rvalue. So we use lvalue here temporary
#define IMPL_ADD_FUNC(type) \
void Add(ecs::Entity entity, type elem) { \
    cmds_.AddComponent<type>(entity, std::move(elem)); \
}

struct CommandsWrapper final {
public:
    friend class SignalManagerWrapper;

    CommandsWrapper(ecs::Commands& cmds): cmds_(cmds) {}

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

void BindCommandsWrapper(LuaScript& script);

class EventsWrapper final {
public:
    friend class SignalManagerWrapper;

    EventsWrapper(ecs::Events& events): events_(events) {}

    // TODO: add events reader and writer

private:
    ecs::Events& events_;
};

void BindEventsWrapper(LuaScript& script);

}