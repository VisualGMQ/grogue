#include "app/lua_manager.hpp"

namespace lua {

LuaScript::LuaScript(LuaScriptHandle handle, const std::string& filename): LuaScript(handle) {
    filename_ = filename;
}

LuaScript::LuaScript(LuaScriptHandle handle): handle_(handle) {
    lua_.open_libraries(sol::lib::base);
}

LuaScript::LuaScript(): handle_(LuaScriptHandle::Null()) { }

LuaScript::~LuaScript() {
    LuaManager::Instance().Destroy(handle_);
}

auto LuaScript::RunCmd(const std::string& cmd) {
    return lua_.script(cmd);
}

auto LuaScript::Execute() {
    if (filename_.empty()) {
        LOGW("try to execute a no-file LuaScript");
        return sol::safe_function_result{};
    } else {
        auto loadResult = lua_.load_file(filename_);
        return loadResult();
    }
}

auto LuaScript::operator[](const std::string& field) const {
    return lua_[field];
}


LuaScriptHandle LuaManager::Load(const std::string& filename) {
    auto handle = LuaScriptHandle::Create();
    datas_[handle] = std::unique_ptr<LuaScript>(new LuaScript{handle, filename});
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

}