#pragma once

#include "core/ecs.hpp"
#include "app/transform.hpp"

//! @brief use for transform in hierarchy
struct LUA_BIND_COMPONENT NodeTransform {
    Transform localTransform;
    Transform globalTransform;
};