#include "app/lua_manager.hpp"

namespace lua {

LuaScript::~LuaScript() {
    LuaManager::Instance().Destroy(handle_);
}


LuaScriptHandle LuaManager::Load(const std::string& filename) {
    auto handle = LuaScriptHandle::Create();
    datas_[handle] =
        std::unique_ptr<LuaScript>(new LuaScript{handle, filename});
    return handle;
}

LuaScript LuaManager::CreateSolitary() {
    return LuaScript{};
}

LuaScriptHandle LuaManager::Create() {
    auto handle = LuaScriptHandle::Create();
    datas_[handle] = std::unique_ptr<LuaScript>(new LuaScript{handle});
    return handle;
}

void LuaManager::Destroy(LuaScriptHandle handle) {
    datas_.erase(handle);
}

LuaScript& LuaManager::Get(LuaScriptHandle handle) {
    return *datas_[handle];
}

}  // namespace lua