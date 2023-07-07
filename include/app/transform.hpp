#pragma once

#include "core/math.hpp"

[[refl, luabind]];
enum Flip {
    None = 0,
    Horizontal = 0x01,
    Vertical = 0x02,
    Both = 0x03,
};

struct [[refl, luabind(comp)]] Transform final {
    math::Vector2 position = math::Vector2::Zero;
    float rotation = 0;  // in degress
    math::Vector2 scale = {1, 1};

    static Transform Create(const math::Vector2& position, float rotation, const math::Vector2& scale) {
        return { position, rotation, scale };
    }

    static Transform FromPosition(const math::Vector2& position) {
        return { position };
    }
    static Transform FromRotation(float rotation) {
        return { {}, rotation };
    }
    static Transform FromScale(const math::Vector2& scale) {
        return { {}, 0, scale };
    }
};