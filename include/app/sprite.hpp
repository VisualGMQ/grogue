#pragma once

#include "app/transform.hpp"
#include "app/fwd.hpp"

struct Sprite final {
    Color color;
    math::Vector2 anchor;
    math::Rect region;
    math::Vector2 customSize;
    Flip flip = Flip::None;

    static Sprite Default() {
        return Sprite{
            Color::White,
            math::Vector2::Zero,
            math::Rect{0, 0, -1, -1},
            math::Vector2{-1, -1},
            Flip::None,
        };
    }

    static Sprite FromRegion(const math::Rect& region) {
        auto default = Default();
        default.region = region;
        return default;
    }

    static Sprite FromCustomSize(const math::Vector2& size) {
        auto default = Default();
        default.customSize = size;
        return default;
    }
};

struct SpriteBundle final {
    Sprite sprite;
    ImageHandle image;
    Transform transform;
    bool visiable = true;
};