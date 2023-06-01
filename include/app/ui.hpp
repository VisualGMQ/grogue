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

//! @brief component for ui rect
struct RectTransform final {
    NodeTransform transform;
    math::Vector2 size;
};

struct Text final {
    FontHandle font;
    std::string text;
    ColorBundle color;
    math::Vector2 offset;
};

struct Image final {
    ImageHandle image;
    math::Rect region;
    ColorBundle color = {Color::White};
    Flip flip = Flip::None;
};

//! @brief component for text
struct Label final {
    Text text;
};

//! @brief component for text
struct Button final {
    ColorBundle contentColor;
    ColorBundle borderColor;
};

using UIEventListener = std::function<void(ecs::Entity, ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&)>;

//! @brief component for ui interaction
struct Interaction final {
    UIEventListener OnClick;
    UIEventListener OnHover;
    UIEventListener OnScroll;
};

void HierarchyRenderLabelSystem(std::optional<ecs::Entity>, ecs::Entity, ecs::Commands&,
                        ecs::Querier, ecs::Resources, ecs::Events&);
void HierarchyRenderButtonSystem(std::optional<ecs::Entity>, ecs::Entity, ecs::Commands&,
                        ecs::Querier, ecs::Resources, ecs::Events&);
void HierarchyHandleUIEventSystem(std::optional<ecs::Entity>, ecs::Entity, ecs::Commands&,
                        ecs::Querier, ecs::Resources, ecs::Events&);
}