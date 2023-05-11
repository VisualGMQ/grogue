#pragma once

#include "core/pch.hpp"
#include "app/handle.hpp"
#include "app/manager.hpp"

class LuaScript;

using LuaScriptHandle = Handle<LuaScript>;

class LuaScript final {
public:
    friend class LuaManager;
    sol::state lua;

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

    //! @brief open lua file or execute lua script from string
    //! @param handle lua handle
    //! @param str lua file name or script content
    //! @param isContent 
    //!     - `true`: str is script content
    //!     - `false`: str is lua filename
    LuaScript(LuaScriptHandle handle, const std::string& str, bool isContent)
        : LuaScript(handle) {
        if (isContent) {
            lua.script(str);
        } else {
            lua.script_file(str);
        }
    }

    friend void swap(LuaScript& lhs, LuaScript& rhs) {
        std::swap(lhs.handle_, rhs.handle_);
        std::swap(lhs.lua, rhs.lua);
    }
};

class LuaManager final : public ResourceManager<LuaScript> {
public:
    LuaScriptHandle Load(const std::string& filename);
    LuaScriptHandle Create();
    LuaScript CreateSolitary();
    LuaScript CreateSolitary(const std::string&);
    LuaScript CreateSolitaryFromContent(const std::string&);
};