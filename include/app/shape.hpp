#pragma once

#include "core/pch.hpp"
#include "app/fwd.hpp"

struct Vertex final {
    math::Vector2 position;
    Color color;
    math::Vector2 texcoord;       /**< Normalized texture coordinates, if needed */
};

//! @brief data for SDL_RenderGeometryRaw
//! @note the size of colors and positions must be same
struct Shape final {
    std::vector<Vertex> vertices;
    std::optional<std::vector<uint32_t>> indices;

    static Shape CreateRect(const math::Vector2& center, const math::Vector2& halfLen, const Color& color);
    static Shape CreateCircle(const math::Vector2& center, float radius, uint32_t slice, const Color& color);
};