#pragma once

#include "app/transform.hpp"
#include "app/fwd.hpp"

//! @brief a component that present a sprite
struct Sprite final {
    Color color = {255, 255, 255, 255};
    math::Rect region;
    math::Vector2 customSize;
    math::Vector2 anchor;

    static Sprite Default() {
        return Sprite{
            Color::White,
            math::Rect{0, 0, -1, -1},
            math::Vector2{-1, -1},
            math::Vector2::Zero,
        };
    }

    static Sprite FromRegion(const math::Rect& region) {
        auto sprite = Sprite::Default();
        sprite.region = region;
        sprite.customSize.Set(region.w, region.h);
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
    Flip flip = Flip::None;
    bool visiable = true;
};
