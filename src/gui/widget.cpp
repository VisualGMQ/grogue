#include "gui/widget.hpp"

namespace grogue::gui {

bool isAnyBtnDown() {
    return core::Mouse::IsPressed(core::MouseButton::Left) ||
           core::Mouse::IsPressed(core::MouseButton::Right)||
           core::Mouse::IsPressed(core::MouseButton::Middle);
}

bool isAnyBtnUp() {
    return core::Mouse::IsReleased(core::MouseButton::Left) ||
           core::Mouse::IsReleased(core::MouseButton::Right)||
           core::Mouse::IsReleased(core::MouseButton::Middle);
}

/*
 * \param target  the align target
 * \param r  output param
 */
void AlignRect(Align align, const core::Rect& target, core::Rect& r) {
    core::Vec2 pos(r.x, r.y);
    if (align & Align::AlignLeft) {
        pos = core::AlignLeftPos(target, r);
        r.x = pos.x; r.y = pos.y;
    }
    if (align & Align::AlignRight) {
        pos = core::AlignRightPos(target, r);
        r.x = pos.x; r.y = pos.y;
    }
    if (align & Align::AlignTop) {
        pos = core::AlignTopPos(target, r);
        r.x = pos.x; r.y = pos.y;
    }
    if (align & Align::AlignBottom) {
        pos = core::AlignBottomPos(target, r);
        r.x = pos.x; r.y = pos.y;
    }
    if (align & Align::AlignXCenter) {
        pos = core::AlignCenterXPos(target, r);
        r.x = pos.x; r.y = pos.y;
    }
    if (align & Align::AlignYCenter) {
        pos = core::AlignCenterYPos(target, r);
        r.x = pos.x; r.y = pos.y;
    }
}

bool Button(bool& show, const char* text, core::Rect* rect, Align align) {
    if (!show) {
        return false;
    }
    auto& renderer = core::VideoMgr::GetMainVideo()->renderer;
    auto* drawRect = rect ? rect : (Context.presentRect ? &Context.presentRect.value() : nullptr);
    if (drawRect) {
        renderer->DrawRect(*drawRect,
                           Context.colorScheme.bgColor,
                           Context.colorScheme.borderColor); 

        auto texture = renderer->GenerateText(*Context.font, text,
                                              Context.colorScheme.fontColor);
        core::Rect textRect(drawRect->x, drawRect->y, texture->GetSize().w, texture->GetSize().h);
        AlignRect(Align::AlignCenter, *drawRect, textRect);
        AlignRect(align,  *drawRect, textRect);
        core::Renderer::TextureRenderInfo info(texture.get());
        info.SetPos(core::Vec2(textRect.x, textRect.y));
        renderer->DrawTexture(info);

        if (core::IsPointInRect(*rect, core::Mouse::Position()) &&
            isAnyBtnDown()) {
        return true;
        }
    }
    return false;
}

}
