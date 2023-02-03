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

    ~LuaScript();

    sol::protected_function_result RunCmd(const std::string& cmd);
    sol::protected_function_result Execute();

    auto operator[](const std::string& field) const;

private:
    sol::state lua_;
    std::string filename_;
    LuaScriptHandle handle_;

    LuaScript(LuaScriptHandle handle, const std::string& filename);
    LuaScript(LuaScriptHandle handle);
    LuaScript();
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