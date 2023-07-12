#pragma once

#include "core/pch.hpp"
#include "app/handle.hpp"
#include "app/manager.hpp"

//! @addtogroup resource-manager
//! @brief some resource class and manager
//! @{

class LuaScript;

using LuaScriptHandle = Handle<LuaScript>;

class [[refl, luabind]] LuaScript final {
public:
    friend class LuaManager;
    sol::state lua;

    LuaScript() = default;
    LuaScript(const LuaScript&) = delete;
    LuaScript(LuaScript&&);
    ~LuaScript();

    LuaScript& operator=(const LuaScript&) = delete;
    LuaScript& operator=(LuaScript&&);

private:
    LuaScriptHandle handle_;

    LuaScript(LuaScriptHandle handle);

    //! @brief open lua file or execute lua script from string
    //! @param handle lua handle
    //! @param str lua file name or script content
    //! @param isContent 
    //!     - `true`: str is script content
    //!     - `false`: str is lua filename
    LuaScript(LuaScriptHandle handle, const std::string& str, bool isContent);

    friend void swap(LuaScript& lhs, LuaScript& rhs) {
        std::swap(lhs.handle_, rhs.handle_);
        std::swap(lhs.lua, rhs.lua);
    }
};

//! @brief manage `LuaScript`
class [[refl, luabind(res)]] LuaManager final : public ResourceManager<LuaScript> {
public:
    //! @brief create a `LuaScript` and load from lua file, will auto destroy when app quit
    //! @return the `Handle` to `LuaScript`
    LuaScriptHandle Load(const std::string& filename);
    //! @brief create a `LuaScript`, will auto destroy when app quit
    //! @return the `Handle` to `LuaScript`
    LuaScriptHandle Create();

    //! @brief create a solitary `LuaScript`, will auto destroy when out of scope
    LuaScript CreateSolitary();

    //! @brief create a solitary `LuaScript` and load from file, will auto destroy when out of scope
    LuaScript CreateSolitary(const std::string&);

    //! @brief create a solitary `LuaScript` and load from lua code, will auto destroy when out of scope
    LuaScript CreateSolitaryFromContent(const std::string&);
};

//! @}