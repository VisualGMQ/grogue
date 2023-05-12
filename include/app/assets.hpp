#pragma once

#include "app/font.hpp"
#include "app/image.hpp"
#include "app/lua.hpp"
#include "app/sound.hpp"


//! @addtogroup resource-manager
//! @{

class AssetsManager final {
public:
    friend class DefaultPlugins;

    //! @brief get image resource manager
    //! @return image resource manager
    //! @see ImageManager
    ImageManager& Image() { return *image_; }

    //! @brief get font resource manager
    //! @return font resource manager
    //! @see FontManager
    FontManager& Font() { return font_; }

    //! @brief get lua resource manager
    //! @return lua resource manager
    //! @see LuaManager
    LuaManager& Lua() { return lua_; }

    //! @brief get audio resource manager
    //! @return audio resource manager
    //! @see BGMMangaer
    BGMManager& BGM() { return bgm_; }

private:
    // can't allocate on stack due to the cycle reference to Renderer
    std::unique_ptr<ImageManager> image_ = nullptr;
    FontManager font_;
    LuaManager lua_;
    BGMManager bgm_;
};