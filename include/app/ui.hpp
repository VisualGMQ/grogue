#pragma once

#include "core/pch.hpp"
#include "app/ui.hpp"
#include "app/renderer.hpp"
#include "app/scene.hpp"
#include "app/input.hpp"
#include "app/assets.hpp"
#include "app/tilesheet.hpp"
#include "app/text_texture.hpp"

namespace ui {

struct LUA_BIND_COMPONENT ColorBundle final {
    Color normal, hover, press;

    static ColorBundle Create(const Color& normal, const Color& hover, const Color& press) {
        return ColorBundle{normal, hover, press};
    }

    static ColorBundle CreatePureColor(const Color& color) {
        return ColorBundle{color, color, color};
    }
};

//! @brief component for ui rect
struct LUA_BIND_COMPONENT RectTransform final {
    NodeTransform transform;
    math::Vector2 size;
};

//! @brief component for ui which want to show a text on it
struct LUA_BIND_COMPONENT Text final {
    std::shared_ptr<TextTexture> text;
    ColorBundle color;
    math::Vector2 offset;

    static Text Create(std::shared_ptr<TextTexture>&& texture, const ColorBundle& color, const math::Vector2& offset) {
        return Text{std::move(texture), color, offset};
    }
};

//! @brief component for ui which want to show a background image on it
struct LUA_BIND_COMPONENT Image final {
    ImageHandle image;
    math::Rect region;
    ColorBundle color = {Color::White};
    Flip flip = Flip::None;

    static Image FromTilesheet(Tilesheet& tilesheet, uint32_t x, uint32_t y) {
        auto tile = tilesheet.Get(x, y);
        Image image;
        image.region = tile.region;
        image.image = tile.handle;
        return image;
    }

    static Image FromSpriteBundle(const SpriteBundle& sprite) {
        Image image;
        image.region = sprite.sprite.region;
        image.image = sprite.image;
        image.color = ColorBundle::CreatePureColor(sprite.sprite.color);
        image.flip = sprite.flip;
        return image;
    }
};

//! @brief component for panel
struct LUA_BIND_COMPONENT Panel final {
    std::optional<ColorBundle> contentColor;
    std::optional<ColorBundle> borderColor;
    bool clipChildren = true;    // if children exceed panel area, clip them

    static Panel Create(const std::optional<ColorBundle>& content, const std::optional<ColorBundle>& border) {
        Panel panel;
        panel.contentColor = content;
        panel.borderColor = border;
        return panel;
    }
};

//! @brief component for ui which want to show a scrolbar and scroll it's content
struct LUA_BIND_COMPONENT Scrollbar final {
    float min;
    float max;
    float value;
    int width;
    int sliderLen;
    float incStep;
    // TODO: add more color config here

    float Ratio() const {
        return value / (max - min);
    }

    static Scrollbar CreateDefault() {
        Scrollbar bar;
        bar.min = 0;
        bar.max = 100;
        bar.value = 0;
        bar.width = 20;
        bar.sliderLen = 20;
        bar.incStep = 5;
        return bar;
    }
};

enum class EventType {
    Click = 0,
    Hover,
    Scroll,

    NUM,    // save event number, isn't a really event
};

//! @brief an event to point out ui event handled
struct EventHandledEvent final {
public:
    void Handled(EventType type) {
        events[static_cast<size_t>(type)] = true;
    }
    bool IsHandled(EventType type) const {
        return events[static_cast<size_t>(type)];
    }
private:
    std::array<bool, static_cast<size_t>(EventType::NUM)> events;
};

struct LUA_BIND UIEventListener {
    using ListenerFunc = std::function<void(ecs::Entity, ecs::Commands&, ecs::Querier,
                       ecs::Resources, ecs::Events&)>;

    bool enable = true;
    ListenerFunc func;

    operator bool() const {
        return enable && func;
    }
};

//! @brief component for ui interaction
struct LUA_BIND_COMPONENT Interaction final {
    UIEventListener& Get(EventType type) {
        return events_[static_cast<size_t>(type)];
    }

    const UIEventListener& Get(EventType type) const {
        return events_[static_cast<size_t>(type)];
    }

    static Interaction Create(UIEventListener::ListenerFunc onClick, UIEventListener::ListenerFunc onHover, UIEventListener:: ListenerFunc onScroll) {
        Interaction result;
        result.events_[static_cast<size_t>(EventType::Click)].func = onClick;
        result.events_[static_cast<size_t>(EventType::Hover)].func = onHover;
        result.events_[static_cast<size_t>(EventType::Scroll)].func = onScroll;
        return result;
    }

private:
    std::array<UIEventListener, static_cast<size_t>(EventType::NUM)> events_;
};

void HierarchyRenderPanelSystem(std::optional<ecs::Entity>, ecs::Entity, ecs::Commands&,
                        ecs::Querier, ecs::Resources, ecs::Events&);
void HierarchyHandleUIEventSystem(std::optional<ecs::Entity>, ecs::Entity, ecs::Commands&,
                        ecs::Querier, ecs::Resources, ecs::Events&);
void HierarchyUpdateScrollbarSystem(std::optional<ecs::Entity>, ecs::Entity, ecs::Commands&,
                        ecs::Querier, ecs::Resources, ecs::Events&);
}