#include "script/lua_manager.hpp"

namespace grogue::script {

// lua function declares
#ifdef __cplusplus
extern "C" {
#endif

int luaFuncDrawLine(lua_State* L);

#ifdef __cplusplus
}
#endif


// lua function implements
int luaFuncDrawLine(lua_State* L) {
    core::Vec2 p1, p2;
    core::Color color;
    p1.x = lua_tonumber(L, 1);
    p1.y = lua_tonumber(L, 2);
    p2.x = lua_tonumber(L, 3);
    p2.y = lua_tonumber(L, 4);
    color.r = lua_tonumber(L, 5);
    color.g = lua_tonumber(L, 6);
    color.b = lua_tonumber(L, 7);
    color.a = lua_tonumber(L, 8);

    core::GFX::DrawLine(p1, p2, color);
    return 0;
}

// regist lua function
static const struct luaL_Reg libscript[] = {
    {"DrawLine", luaFuncDrawLine},
    {NULL, NULL}
};

extern "C" int luaopen_libscript(lua_State* L){
    luaL_newlib(L, libscript);
    lua_pushvalue(L, -1);
    return 1;
}

lua_State* LuaMgr::L_ = nullptr;

void LuaMgr::Init(const char* filename) {
    L_ = luaL_newstate();
    luaL_openlibs(L_);
    if (luaL_loadfile(L_, filename) || lua_pcall(L_, 0, 0, 0)) {
        LOG_ERROR("can't run lua file %s: %s", filename, lua_tostring(L_, -1));
        lua_close(L_);
        L_ = nullptr;
    }
}

void LuaMgr::Shutdown() {
    if (L_) {
        lua_close(L_);
    }
}

void LuaMgr::Update() {
    if (L_) {
        lua_getglobal(L_, "Update");
        if (lua_pcall(L_, 0, 0, 0) != 0) {
            LOG_ERROR("error execute function Update(): %s", lua_tostring(L_, -1));
        }
    }
}

}
