#pragma once

#include "SDL_ttf.h"
#include "app/handle.hpp"
#include "core/log.hpp"
#include "app/manager.hpp"
#include <string>

class Font;

using FontHandle = Handle<Font>;

class Font final {
public:
    friend class FontManager;

    Font(const Font&) = delete;
    Font(Font&&);
    ~Font();

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

class FontManager final : public Manager<Font> {
public:
    FontHandle Load(const std::string& filename, int ptsize);
};