#include "app/ui.hpp"

namespace ui {

const Color* selectColor(const math::Rect& rect, const math::Vector2& mousePos, ecs::Events& events, const ColorBundle& colors) {
   const Color* color = &colors.normal;
    if (rect.ContainPt(mousePos)) {
        color = &colors.hover;

        auto reader = events.Reader<MouseBtnEvents>();
        if (reader && reader.Read().events[0].button == SDL_BUTTON_LEFT) {
            color = &colors.press;
        }
    }
    return color;
}

void drawText(Renderer& renderer, Text& text, const NodeTransform& transform, const Color& color) {
    renderer.SetDrawColor(color);
    Transform drawTrans = transform.globalTransform;
    drawTrans.position += text.offset * drawTrans.scale;
    SpriteBundleSolitary spriteBundle {
        Sprite::Default(),
        *text.text->GetImage(),
    };
    renderer.DrawSprite(spriteBundle, drawTrans);
}

inline math::Rect rectTransform2Rect(const RectTransform& transform) {
    return math::Rect::Create(
            transform.transform.globalTransform.position,
            transform.transform.globalTransform.scale * transform.size);
}

void tryResetUIRenderStateAtRoot(std::optional<ecs::Entity> parent,
                              ecs::Entity entity, ecs::Querier querier,
                              ecs::Resources res) {
    if (!parent && querier.Has<RectTransform>(entity)) {
        auto& renderer = res.Get<Renderer>();
        renderer.SetDefaultClipArea();
    }
}

//! @brief judge children's rect area in parent rect area
inline bool isChildrenInParent(ecs::Entity parent, ecs::Entity entity, ecs::Querier querier) {
    if (querier.Has<RectTransform>(parent) && querier.Has<RectTransform>(entity)) {
        auto parentRect = rectTransform2Rect(querier.Get<RectTransform>(parent));
        auto rect = rectTransform2Rect(querier.Get<RectTransform>(entity));
        return parentRect.IsIntersect(rect);
    }
    return false;
}

inline void tryClipArea(ecs::Entity entity, ecs::Querier querier, Renderer& renderer) {
    if (querier.Has<RectTransform>(entity) &&
        querier.Has<Panel>(entity)) {
        auto& parentTrans = querier.Get<RectTransform>(entity);
        auto& panel = querier.Get<Panel>(entity);
        auto rect = rectTransform2Rect(parentTrans);
        if (panel.clipChildren) {
            math::Rect clipRect = rect;
            // border will not clip
            clipRect.x += 1;
            clipRect.y += 1;
            clipRect.w -= 2;
            clipRect.h -= 2;
            renderer.SetClipArea(clipRect);
        } else {
            renderer.SetDefaultClipArea();
        }
    }
}

void HierarchyRenderPanelSystem(std::optional<ecs::Entity> parent,
                                ecs::Entity entity, ecs::Commands& cmds,
                                ecs::Querier querier, ecs::Resources res,
                                ecs::Events& events) {
    if (parent && !isChildrenInParent(parent.value(), entity, querier)) {
        return;
    }

    if (!querier.Has<RectTransform>(entity) || !querier.Has<Panel>(entity)) {
        return;
    }

    tryResetUIRenderStateAtRoot(parent, entity, querier, res);

    auto& renderer = res.Get<Renderer>();
    if (parent) {
        tryClipArea(parent.value(), querier, renderer);
    }

    if (parent && querier.Has<NodeTransform>(parent.value()) &&
        querier.Has<Panel>(parent.value())) {
        auto& parentTrans = querier.Get<RectTransform>(parent.value());
        auto& panel = querier.Get<Panel>(parent.value());
        auto rect = rectTransform2Rect(parentTrans);
        if (panel.clipChildren) {
            math::Rect clipRect = rect;
            // border will not clip
            clipRect.x += 1;
            clipRect.y += 1;
            clipRect.w -= 2;
            clipRect.h -= 2;
            renderer.SetClipArea(clipRect);
        } else {
            renderer.SetDefaultClipArea();
        }
    }

    auto& transform = querier.Get<RectTransform>(entity);
    auto& panel = querier.Get<Panel>(entity);
    auto& fontMgr = res.Get<AssetsManager>().Font();
    auto& mouse = res.Get<Mouse>();
    auto rect = rectTransform2Rect(transform);

    if (panel.contentColor) {
        auto contentColor =
            selectColor(rect, mouse.Position(), events, panel.contentColor.value());
        renderer.SetDrawColor(*contentColor);
        renderer.FillRect(rect);
    }
    if (panel.borderColor) {
        auto borderColor =
            selectColor(rect, mouse.Position(), events, panel.borderColor.value());
        renderer.SetDrawColor(*borderColor);
        renderer.DrawRect(rect);
    }

    if (querier.Has<Image>(entity)) {
        auto& image = querier.Get<Image>(entity);
        auto color = selectColor(rect, mouse.Position(), events, image.color);
        SpriteBundle bundle;
        bundle.flip = image.flip;
        bundle.image = image.image;
        bundle.sprite.color = *color;
        bundle.sprite.region = image.region;
        bundle.sprite.customSize = transform.size;
        renderer.DrawSprite(bundle, transform.transform.globalTransform);
    }

    if (querier.Has<Text>(entity)) {
        auto& text = querier.Get<Text>(entity);
        auto& fontMgr = res.Get<AssetsManager>().Font();
        auto color = selectColor(rect, mouse.Position(), events, text.color);
        drawText(renderer, text, transform.transform, *color);
    }

    if (querier.Has<Scrollbar>(entity)) {
        auto& scrollbar = querier.Get<Scrollbar>(entity);
        math::Rect barRect = math::Rect::Create(rect.x + rect.w, rect.y,
                                                scrollbar.width, rect.h);
        renderer.SetDrawColor(Color::White);
        renderer.FillRect(barRect);
        renderer.SetDrawColor(Color::Black);
        renderer.DrawRect(barRect);

        float offset = scrollbar.Ratio() * (rect.h - scrollbar.sliderLen);

        math::Rect slideRect = math::Rect::Create(
            barRect.x, barRect.y + offset, barRect.w, scrollbar.sliderLen);
        renderer.SetDrawColor(Color{150, 150, 150});
        renderer.FillRect(slideRect);
        renderer.SetDrawColor(Color::Black);
        renderer.DrawRect(slideRect);
    }
}

void HierarchyHandleUIEventSystem(std::optional<ecs::Entity> parent,
                                  ecs::Entity entity, ecs::Commands& cmds,
                                  ecs::Querier querier, ecs::Resources res,
                                  ecs::Events& events) {
    if (!querier.Has<RectTransform>(entity) ||
        !querier.Has<Interaction>(entity)) {
        return;
    }

    auto& transform = querier.Get<RectTransform>(entity);
    auto& mouse = res.Get<Mouse>();
    auto rect = rectTransform2Rect(transform);

    if (parent && querier.Has<RectTransform>(parent.value()) &&
        querier.Has<Panel>(parent.value())) {
        auto& parentTrans = querier.Get<RectTransform>(parent.value());
        auto& parentPanel = querier.Get<Panel>(parent.value());

        auto parentRect = rectTransform2Rect(parentTrans);
        auto unionRect = rect.Union(parentRect);
        if (!unionRect) {
            return;
        }
        rect = unionRect.value();
    }

    std::optional<ecs::Entity> mouseHoverChild;
    if (!rect.ContainPt(mouse.Position())) {
        return;
    }

    auto& node = querier.Get<Node>(entity);

    for (auto child : node.children) {
        if (querier.Has<RectTransform>(child)) {
            auto& transform = querier.Get<RectTransform>(child);
            auto childRect = rectTransform2Rect(transform);
            auto unionRect = rect.Union(childRect);
            if (unionRect && unionRect->ContainPt(mouse.Position())) {
                // mouse hover on child ui, so we can't handle this ui's event
                mouseHoverChild = child;
                break;
            }
        }
    }

    auto& interaction = querier.Get<Interaction>(entity);
    auto handledReader = events.Reader<EventHandledEvent>();
    auto reader = events.Reader<MouseBtnEvents>();

    // some temporary macro to help judge event handlable

#define IsHandled(event) \
    (handledReader && handledReader.Read().IsHandled(EventType::Click))

#define CanChildHandle(event)                                                \
    (mouseHoverChild && querier.Has<Interaction>(mouseHoverChild.value()) && \
     querier.Get<Interaction>(mouseHoverChild.value()).Get(event))

#define CanHandleEvent(event) (!CanChildHandle(event) && !IsHandled(event))

// a temporary macro to mark handled event
#define MarkHandledEvent(event)                                             \
    do {                                                                    \
        EventHandledEvent data;                                             \
        if (handledReader) {                                                \
            EventHandledEvent newData = handledReader.Read();               \
        }                                                                   \
        data.Handled(event);                                                \
        events.Writer<EventHandledEvent>().WriteImmediate(std::move(data)); \
    } while (0)

    if (reader && reader.Read().events[0].button == SDL_BUTTON_LEFT &&
        reader.Read().events[0].type == SDL_MOUSEBUTTONDOWN) {
        if (CanHandleEvent(EventType::Click)) {
            if (auto& handle = interaction.Get(EventType::Click); handle) {
                handle.func(entity, cmds, querier, res, events);
                MarkHandledEvent(EventType::Click);
            }
        }
    }

    if (CanHandleEvent(EventType::Hover)) {
        if (auto& handle = interaction.Get(EventType::Hover); handle) {
            handle.func(entity, cmds, querier, res, events);
            MarkHandledEvent(EventType::Hover);
        }
    }

    auto wheelReader = events.Reader<MouseWheelEvent>();
    if (wheelReader && CanHandleEvent(EventType::Scroll)) {
        auto& wheel = wheelReader.Read();
        if (auto& handle = interaction.Get(EventType::Scroll); handle) {
            handle.func(entity, cmds, querier, res, events);
            MarkHandledEvent(EventType::Scroll);
        }

        if (querier.Has<Scrollbar>(entity)) {
            auto& scrollbar = querier.Get<Scrollbar>(entity);
            scrollbar.value = std::clamp(
                scrollbar.value -
                    scrollbar.incStep *
                        (wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1 : 1) *
                        wheel.preciseY,
                scrollbar.min, scrollbar.max);
        }
    }

#undef CanHandleEvent
#undef MarkHandledEvent
#undef IsHandled
}

void HierarchyUpdateScrollbarSystem(std::optional<ecs::Entity>,
                                    ecs::Entity entity, ecs::Commands&,
                                    ecs::Querier querier, ecs::Resources,
                                    ecs::Events&) {
    if (!querier.Has<RectTransform>(entity) ||
        !querier.Has<Scrollbar>(entity)) {
        return;
    }

    auto& transform = querier.Get<RectTransform>(entity);
    auto rect = rectTransform2Rect(transform);

    if (querier.Has<Scrollbar>(entity)) {
        auto& scrollbar = querier.Get<Scrollbar>(entity);
        auto& node = querier.Get<Node>(entity);
        for (auto child : node.children) {
            if (querier.Has<RectTransform>(child)) {
                auto& transform = querier.Get<RectTransform>(child);
                transform.transform.globalTransform.position.y -=
                    scrollbar.Ratio() * rect.h;
            }
        }
    }
}

}  // namespace ui
