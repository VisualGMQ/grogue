#pragma once

#include "core/pch.hpp"
#include "app/lua.hpp"
#include "app/app.hpp"
#include "luabind/component_bind.hpp"
#include "luabind/commands_bind.hpp"


namespace lua_bind {

class SignalManagerWrapper;

class ResourcesWrapper final {
public:
    friend class SignalManagerWrapper;

    ResourcesWrapper(ecs::Resources res): res_(res) {}
    ::Keyboard& GetKeyboard();
    ::Mouse& GetMouse();
    ::Time& GetTime();
    ::TilesheetManager& GetTilesheetManager();
    ::Renderer& GetRenderer();
    ::ImageManager& GetImageManager();
    ::FontManager & GetFontManager();
    SignalManagerWrapper GetSignalManager();
    ::LuaShareContext& GetLuaShareContext();

private:
    ecs::Resources res_;
};

class SignalManagerWrapper final {
public:
    SignalManagerWrapper(SignalManager& raw): raw_(raw) {}

    void Remove(uint32_t name) {
        raw_.Remove(name);
    }

    void Raise(uint32_t name, CommandsWrapper cmds, QuerierWrapper querier, ResourcesWrapper res, EventsWrapper events) {
        raw_.Raise(name, cmds.cmds_, querier.querier_, res.res_, events.events_);
    }


    void Regist(uint32_t name, ::SignalManager::CallbackFunc func) {
        raw_.Regist(name, func);
    }

private:
    ::SignalManager& raw_;
};

void BindResourcesWrapper(::LuaScript& script);
void BindResources(::LuaScript& script);

}