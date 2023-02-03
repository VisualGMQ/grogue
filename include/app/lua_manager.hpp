#pragma once

#include <string>

#include "app/handle.hpp"
#include "core/singlton.hpp"

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

    ~LuaScript();
    LuaScript() : handle_(LuaScriptHandle::Null()) {}

private:
    LuaScriptHandle handle_;

    LuaScript(LuaScriptHandle handle): handle_(handle) {
        lua.open_libraries(sol::lib::base);
    }
    LuaScript(LuaScriptHandle handle, const std::string& filename): LuaScript(handle) {
        lua.script_file(filename);
    }
};

class LuaManager final : public Singlton<LuaManager, false> {
public:
    LuaScriptHandle Load(const std::string& filename);
    LuaScriptHandle Create();
    LuaScript CreateSolitary();
    void Destroy(LuaScriptHandle);
    LuaScript& Get(LuaScriptHandle);

private:
    std::unordered_map<LuaScriptHandle, std::unique_ptr<LuaScript>, LuaScriptHandle::Hash, LuaScriptHandle::HashEq> datas_;
};

}  // namespace lua