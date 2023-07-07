#pragma once

#include "core/ecs.hpp"
#include "app/transform.hpp"

//! @brief use for transform in hierarchy
struct [[refl, luabind(comp)]] NodeTransform {
    Transform localTransform;
    Transform globalTransform;
};