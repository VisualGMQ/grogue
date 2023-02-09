#pragma once

#include "app/font.hpp"
#include "app/image.hpp"
#include "app/lua.hpp"
#include "app/sound.hpp"

class AssetsManager final {
public:
    friend class DefaultPlugins;

    ImageManager& Image() { return *image_; }

    FontManager& Font() { return font_; }

    lua::LuaManager& Lua() { return lua_; }

    BGMManager& BGM() { return bgm_; }

private:
    // can't allocate on stack due to the cycle reference to Renderer
    std::unique_ptr<ImageManager> image_ = nullptr;
    FontManager font_;
    lua::LuaManager lua_;
    BGMManager bgm_;
};