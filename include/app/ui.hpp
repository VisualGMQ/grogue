#pragma once

#include "core/pch.hpp"
#include "app/ui.hpp"
#include "app/renderer.hpp"
#include "app/scene.hpp"
#include "app/input.hpp"
#include "app/assets.hpp"

namespace ui {

struct ColorBundle final {
    Color normal, hover, press;
};

//! @brief a tag component for all ui widget
struct Widget final {
    math::Vector2 size;
    std::optional<ColorBundle> content, border;

    static Widget CreateDefault(const math::Vector2& size);
};

//! @brief button widget
struct Button final {
    FontHandle font;
    Color fontColor;
    std::string text;

    ecs::HierarchyUpdateSystem system;  // IMPROVE: may change this function, due to the ducplicate query in function

    static Button CreateDefault(const std::string& text, FontHandle font, const Color& fontColor);
};

void RenderBackSystem(std::optional<ecs::Entity>, ecs::Entity,
                      ecs::Commands&, ecs::Querier, ecs::Resources,
                      ecs::Events&);

void RenderButtonSystem(std::optional<ecs::Entity>, ecs::Entity, ecs::Commands&,
                        ecs::Querier, ecs::Resources, ecs::Events&);
}