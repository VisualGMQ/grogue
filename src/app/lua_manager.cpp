#include "app/lua_manager.hpp"

namespace lua {

LuaScript::~LuaScript() {
    LuaScriptHandle::Destroy(handle_);
}


LuaScriptHandle LuaManager::Load(const std::string& filename) {
    auto handle = LuaScriptHandle::Create();
    auto item = std::unique_ptr<LuaScript>(new LuaScript{handle, filename});
    storeNewItem(handle, item);
    return handle;
}

LuaScript LuaManager::CreateSolitary() {
    return LuaScript{};
}

LuaScriptHandle LuaManager::Create() {
    auto handle = LuaScriptHandle::Create();
    auto item = std::unique_ptr<LuaScript>(new LuaScript{handle});
    storeNewItem(handle, item);
    return handle;
}

}  // namespace lua