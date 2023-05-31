#include "app/ui.hpp"

namespace ui {

void RenderBackSystem(std::optional<ecs::Entity>, ecs::Entity entity,
                         ecs::Commands&, ecs::Querier querier,
                         ecs::Resources res, ecs::Events& events) {
    auto& renderer = res.Get<Renderer>();
    auto& mouse = res.Get<Mouse>();

    Transform* transform = nullptr;
    if (querier.Has<NodeTransform>(entity)) {
        transform = &querier.Get<NodeTransform>(entity).localTransform;
    } else if (querier.Has<Transform>(entity)) {
        transform = &querier.Get<Transform>(entity);
    } else {
        return;
    }

    auto& widget = querier.Get<Widget>(entity);
    auto rect = math::Rect::Create(transform->position, widget.size);

    // IMPROVE: too many (?:), simplify it
    const Color* contentColor = widget.content ? &widget.content.value().normal : nullptr;
    const Color* borderColor = widget.border ? &widget.border.value().normal : nullptr;
    if (rect.ContainPt(mouse.Position())) {
        contentColor = widget.content ? &widget.content.value().hover : nullptr;
        borderColor = widget.border ? &widget.border.value().hover : nullptr;
        if (mouse.LeftBtn().IsPressing()) {
            contentColor = widget.content ? &widget.content.value().press: nullptr;
            borderColor = widget.border ? &widget.border.value().press: nullptr;
        }
    }

    if (contentColor) {
        renderer.SetDrawColor(*contentColor);
        renderer.FillRect(rect);
    }

    if (borderColor) {
        renderer.SetDrawColor(*borderColor);
        renderer.DrawRect(rect);
    }
}

void RenderButtonSystem(std::optional<ecs::Entity> parent, ecs::Entity entity,
                        ecs::Commands& cmds, ecs::Querier querier,
                        ecs::Resources res,
                        ecs::Events& events){
    RenderBackSystem(parent, entity, cmds, querier, res, events);

    Transform* transform = nullptr;
    if (querier.Has<NodeTransform>(entity)) {
        transform = &querier.Get<NodeTransform>(entity).localTransform;
    } else if (querier.Has<Transform>(entity)) {
        transform = &querier.Get<Transform>(entity);
    } else {
        return;
    }
    auto& renderer = res.Get<Renderer>();
    auto& widget = querier.Get<Widget>(entity);
    auto rect = math::Rect::Create(transform->position, widget.size);

    auto& button = querier.Get<Button>(entity);
    auto& fontMgr = res.Get<AssetsManager>().Font();
    auto& font = fontMgr.Get(button.font);
    auto fontSize = font.Size(button.text);
    Transform fontTrans = *transform;
    fontTrans.position = (widget.size - fontSize) * 0.5 + transform->position;
    renderer.SetDrawColor(button.fontColor);
    renderer.DrawText(button.font, button.text, fontTrans);
}

Widget Widget::CreateDefault(const math::Vector2& size) {
    return Widget{
        size,
        ColorBundle{Color::Green,   Color::Red,  Color::Blue},
        ColorBundle{Color::Black, Color::Black, Color::Black},
    };
}

Button Button::CreateDefault(const std::string& text, FontHandle font, const Color& fontColor) {
    return Button{
        font,
        fontColor,
        text,
        {RenderButtonSystem},
    };
}

}  // namespace ui
