#pragma once

#include "app/events.hpp"
#include "app/handle.hpp"
#include "core/pch.hpp"
#include "core/math.hpp"

class Image;
using ImageHandle = Handle<Image>;

struct [[refl, luabind]] Color {
    uint8_t r, g, b, a;

    static Color White;
    static Color Black;
    static Color Red;
    static Color Blue;
    static Color Green;

    Color() : r(0), g(0), b(0), a(255) {}

    Color(unsigned char r, unsigned char g, unsigned char b,
          unsigned char a = 255)
        : r(r), g(g), b(b), a(a) {}
};

struct [[refl, luabind]] Tile final {
    math::Rect region;
    ImageHandle handle;
};