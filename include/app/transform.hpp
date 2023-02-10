#pragma once

#include "core/math.hpp"

enum Flip {
    None = 0,
    Horizontal = 0x01,
    Vertical = 0x02,
    Both = Vertical | Horizontal,
};

struct Transform final {
    math::Vector2 position{math::Vector2::Zero};
    float rotation = 0;  // in radians
    math::Vector2 scale{1, 1};
};