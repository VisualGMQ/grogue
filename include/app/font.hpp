#pragma once

#include "core/pch.hpp"
#include "app/handle.hpp"
#include "app/manager.hpp"


//! @addtogroup resource-manager
//! @{

class Font;

using FontHandle = Handle<Font>;

class [[refl, luabind]] Font final {
public:
    friend class FontManager;
    friend class Renderer;

    Font(const Font&) = delete;
    Font(Font&&);
    ~Font();

    //! @brief predict the size of font image
    math::Vector2 Size(const std::string&);
    void SetPt(int ptsize);

    Font& operator=(const Font&) = delete;
    Font& operator=(Font&&);

private:
    FontHandle handle_;
    TTF_Font* font_;

    Font(FontHandle, const std::string& filename, int ptsize);

    friend void swap(Font& lhs, Font& rhs) {
        std::swap(lhs.font_, rhs.font_);
        std::swap(lhs.handle_, rhs.handle_);
    }
};

class [[refl, luabind(res)]] FontManager final : public ResourceManager<Font> {
public:
    FontHandle Load(const std::string& filename, int ptsize);
};

//! @}