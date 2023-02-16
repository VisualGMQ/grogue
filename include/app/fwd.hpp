#pragma once

#include "app/handle.hpp"
#include "core/pch.hpp"
#include "core/math.hpp"

class Image;
using ImageHandle = Handle<Image>;

template <typename T>
struct DuplicateEvent {
    std::vector<T> events;

    operator bool() const {
        return !events.empty();
    }
};

using KeyboardEvents = DuplicateEvent<SDL_KeyboardEvent>;
using MouseBtnEvents = DuplicateEvent<SDL_MouseButtonEvent>;

struct Color {
    unsigned char r, g, b, a;

    static Color White;
    static Color Black;

    Color() : r(0), g(0), b(0), a(255) {}

    Color(unsigned char r, unsigned char g, unsigned char b,
          unsigned char a = 255)
        : r(r), g(g), b(b), a(a) {}
};

struct Tile final {
    math::Rect region;
    ImageHandle handle;
};