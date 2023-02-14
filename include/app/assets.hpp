#pragma once

#include "app/font.hpp"
#include "app/image.hpp"
#include "app/lua.hpp"
#include "app/sound.hpp"

class AssetsManager final {
public:
    friend class DefaultPlugins;

    AssetsManager(const std::string& rootDir) {
        font_.reset(new FontManager(rootDir));
        lua_.reset(new LuaManager(rootDir));
        bgm_.reset(new BGMManager(rootDir));
    }

    ImageManager& Image() { return *image_; }

    FontManager& Font() { return *font_; }

    LuaManager& Lua() { return *lua_; }

    BGMManager& BGM() { return *bgm_; }

private:
    // can't allocate on stack due to the cycle reference to Renderer
    std::unique_ptr<ImageManager> image_ = nullptr;
    std::unique_ptr<FontManager> font_;
    std::unique_ptr<LuaManager> lua_;
    std::unique_ptr<BGMManager> bgm_;
};