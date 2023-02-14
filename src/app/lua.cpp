#include "app/lua.hpp"

LuaScript::~LuaScript() {
    LuaScriptHandle::Destroy(handle_);
}

LuaScript::LuaScript(LuaScript&& o): handle_(LuaScriptHandle::Null()) {
    std::swap(*this, o);
}

LuaScript& LuaScript::operator=(LuaScript&& o) {
    if (&o != this) {
        std::swap(*this, o);
    }
    return *this;
}


LuaScriptHandle LuaManager::Load(const std::string& filename) {
    auto handle = LuaScriptHandle::Create();
    storeNewItem(handle, std::unique_ptr<LuaScript>(new LuaScript{handle, GetRootDir() + filename}));
    return handle;
}

LuaScriptHandle LuaManager::Create() {
    auto handle = LuaScriptHandle::Create();
    storeNewItem(handle, std::unique_ptr<LuaScript>(new LuaScript{handle}));
    return handle;
}

LuaScript LuaManager::CreateSolitary() {
    return LuaScript{LuaScriptHandle::Null()};
}

LuaScript LuaManager::CreateSolitary(const std::string& filename) {
    return LuaScript{LuaScriptHandle::Null(), GetRootDir() + filename};
}