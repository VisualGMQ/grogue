自动绑定Lua脚本的工具。会解析所有可绑定的类（绑定所有类中的public成员）。需要在绑定的结构体前加`LUA_XXX`来生效（具体请看`../../include/core/luabind_tag.hpp`）

有一些类可能无法自动识别（比如模板），这时你可以在`./luabidn.cpp.tmpl`中手动绑定