#include "app/ui.hpp"

namespace ui {

enum class EventType {
    None,
    Hover,
    Click,
};

EventType judgeEvent(const RectTransform& transform, const Mouse& mouse, ecs::Events& events) {
    EventType event = EventType::None;
    auto rect = math::Rect::Create(
            transform.transform.globalTransform.position,
            transform.transform.globalTransform.scale * transform.size);
    if (rect.ContainPt(mouse.Position())) {
        event = EventType::Hover;

        auto reader = events.Reader<MouseBtnEvents>();
        if (reader && reader.Read().events[0].button == SDL_BUTTON_LEFT) {
            event = EventType::Click;
        }
    }
    return event;
}

const Color* selectColor(EventType event, const ColorBundle colors) {
    const Color* color = &colors.normal;
    switch (event) {
        case EventType::Hover:
            color = &colors.hover;
            break;
        case EventType::Click:
            color = &colors.press;
            break;
    }
    return color;
}

void drawText(Renderer& renderer, Font& font, const NodeTransform& transform, const Text& text, const Color& color) {
    renderer.SetDrawColor(color);
    Transform drawTrans = transform.globalTransform;
    drawTrans.position += text.offset * drawTrans.scale;
    renderer.DrawText(font, text.text, drawTrans);
}

void HierarchyRenderLabelSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                                ecs::Commands& cmds, ecs::Querier querier,
                                ecs::Resources res, ecs::Events& events) {
    if (!querier.Has<RectTransform>(entity) || !querier.Has<Label>(entity)) {
        return;
    }

    auto& transform = querier.Get<RectTransform>(entity);
    auto& label = querier.Get<Label>(entity);
    auto& fontMgr = res.Get<AssetsManager>().Font();
    auto& renderer = res.Get<Renderer>();
    auto& mouse = res.Get<Mouse>();

    auto event = judgeEvent(transform, mouse, events);
    auto color = selectColor(event, label.text.color);

    auto& font = fontMgr.Get(label.text.font);
    drawText(renderer, font, transform.transform, label.text, *color);
}

void HierarchyRenderButtonSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                                 ecs::Commands& cmds, ecs::Querier querier, ecs::Resources res,
                                 ecs::Events& events) {
    if (!querier.Has<RectTransform>(entity) || !querier.Has<Button>(entity)) {
        return;
    }

    auto& transform = querier.Get<RectTransform>(entity);
    auto& button = querier.Get<Button>(entity);
    auto& fontMgr = res.Get<AssetsManager>().Font();
    auto& renderer = res.Get<Renderer>();
    auto& mouse = res.Get<Mouse>();

    auto event = judgeEvent(transform, mouse, events);
    auto contentColor = selectColor(event, button.contentColor);
    auto borderColor = selectColor(event, button.borderColor);

    auto rect = math::Rect::Create(
            transform.transform.globalTransform.position,
            transform.transform.globalTransform.scale * transform.size);

    renderer.SetDrawColor(*contentColor);
    renderer.FillRect(rect);
    renderer.SetDrawColor(*borderColor);
    renderer.DrawRect(rect);
    
    if (querier.Has<Image>(entity)) {
        auto& image = querier.Get<Image>(entity);
        auto color = selectColor(event, image.color);
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
        auto& font = fontMgr.Get(text.font);
        auto color = selectColor(event, text.color);
        drawText(renderer, font, transform.transform, text, *color);
    }
}

inline math::Rect rectTransform2Rect(const RectTransform& transform) {
    return math::Rect::Create(
        transform.transform.globalTransform.position,
        transform.transform.globalTransform.scale * transform.size);
}

void HierarchyHandleUIEventSystem(std::optional<ecs::Entity>, ecs::Entity entity, ecs::Commands& cmds,
                        ecs::Querier querier, ecs::Resources res, ecs::Events& events) {
    if (!querier.Has<RectTransform>(entity)) {
        return;
    }

    auto& transform = querier.Get<RectTransform>(entity);
    auto& mouse = res.Get<Mouse>();
    auto rect = rectTransform2Rect(transform);

    if (querier.Has<Interaction>(entity) && rect.ContainPt(mouse.Position())) {
        auto& node = querier.Get<Node>(entity);

        for (auto child : node.children) {
            if (querier.Has<RectTransform>(child)) {
                auto& transform = querier.Get<RectTransform>(child);
                auto childRect = rectTransform2Rect(transform);
                auto unionRect = rect.Union(childRect);
                if (unionRect && unionRect->ContainPt(mouse.Position())) {
                    // mouse hover on child ui, so we can't deal this ui's event
                    return;
                }
            }
        }

        auto& interaction = querier.Get<Interaction>(entity);
        if (rect.ContainPt(mouse.Position())) {
            auto reader = events.Reader<MouseBtnEvents>();
            if (reader && reader.Read().events[0].button == SDL_BUTTON_LEFT &&
                reader.Read().events[0].type == SDL_MOUSEBUTTONDOWN) {
                if (interaction.OnClick) interaction.OnClick(entity, cmds, querier, res, events);
            } else {
                if (interaction.OnHover) interaction.OnHover(entity, cmds, querier, res, events);
            }
        }
    }
}

}  // namespace ui
