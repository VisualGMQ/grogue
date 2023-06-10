#include "app/lua.hpp"

LuaScript::LuaScript(LuaScriptHandle handle) : handle_(handle) {
    lua.open_libraries(sol::lib::base);
    lua.open_libraries(sol::lib::package);
    lua.open_libraries(sol::lib::debug);
}

LuaScript::LuaScript(LuaScriptHandle handle, const std::string& str, bool isContent)
    : LuaScript(handle) {
    if (isContent) {
        lua.script(str);
        sol::load_result result = lua.load_file(str);
    } else {
        lua.script_file(str);
    }
}


LuaScript::~LuaScript() {
    LuaScriptHandle::Destroy(handle_);
}

LuaScript::LuaScript(LuaScript&& o): handle_(LuaScriptHandle::Null()) {
    swap(*this, o);
}

LuaScript& LuaScript::operator=(LuaScript&& o) {
    if (&o != this) {
        swap(*this, o);
    }
    return *this;
}


LuaScriptHandle LuaManager::Load(const std::string& filename) {
    auto handle = LuaScriptHandle::Create();
    storeNewItem(handle, std::unique_ptr<LuaScript>(new LuaScript{handle, filename, false}));
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
    return LuaScript{LuaScriptHandle::Null(), filename, false};
}

LuaScript LuaManager::CreateSolitaryFromContent(const std::string& content) {
    return LuaScript{LuaScriptHandle::Null(), content, true};
}