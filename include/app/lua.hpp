#pragma once

#include <string>

#include "app/handle.hpp"
#include "core/singleton.hpp"
#include "app/manager.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

#include "core/log.hpp"

namespace lua {

class LuaScript;

using LuaScriptHandle = Handle<LuaScript>;

class LuaScript final {
public:
    friend class LuaManager;
    sol::state lua;

    LuaScript() : handle_(LuaScriptHandle::Null()) {}
    LuaScript(const LuaScript&) = delete;
    LuaScript(LuaScript&&);
    ~LuaScript();

    LuaScript& operator=(const LuaScript&) = delete;
    LuaScript& operator=(LuaScript&&);

private:
    LuaScriptHandle handle_;

    LuaScript(LuaScriptHandle handle) : handle_(handle) {
        lua.open_libraries(sol::lib::base);
    }

    LuaScript(LuaScriptHandle handle, const std::string& filename)
        : LuaScript(handle) {
        lua.script_file(filename);
    }

    friend void swap(LuaScript& lhs, LuaScript& rhs) {
        std::swap(lhs.handle_, rhs.handle_);
        std::swap(lhs.lua, rhs.lua);
    }
};

class LuaManager final : public Manager<LuaScript> {
public:
    LuaScriptHandle Load(const std::string& filename);
    LuaScriptHandle Create();
    LuaScript CreateSolitary();
};

}  // namespace lua