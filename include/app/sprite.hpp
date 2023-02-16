#pragma once

#include "app/transform.hpp"
#include "app/fwd.hpp"

struct Sprite final {
    Color color;
    math::Rect region;
    math::Vector2 customSize;
    math::Vector2 anchor;
    Flip flip = Flip::None;

    static Sprite Default() {
        return Sprite{
            Color::White,
            math::Rect{0, 0, -1, -1},
            math::Vector2{-1, -1},
            math::Vector2::Zero,
            Flip::None,
        };
    }

    static Sprite FromRegion(const math::Rect& region) {
        auto sprite = Sprite::Default();
        sprite.region = region;
        return sprite;
    }

    static Sprite FromCustomSize(const math::Vector2& size) {
        auto sprite = Sprite::Default();
        sprite.customSize = size;
        return sprite;
    }
};

struct SpriteBundle final {
    Sprite sprite;
    ImageHandle image;
    Transform transform;
    bool visiable = true;
};
