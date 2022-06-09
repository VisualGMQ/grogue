#pragma once

#include "lua.hpp"
#include "core/gfx.hpp"

namespace grogue::script {

class LuaMgr final {
public:
    static void Init(const char* filename);
    static void Shutdown();
    static void Update();

private:
    static lua_State* L_;
};

}
