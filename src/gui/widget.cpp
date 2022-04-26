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


enum Event {
    NoEvent     = 0,
    MouseUp     = 0x00000002,
    MouseDown   = 0x00000004,
    MouseMotion = 0x00000008,
    KeyDown     = 0x00000010,
    KeyUp       = 0x00000020,
};

Event detectEvent(const core::Rect& r) {
    if (core::IsPointInRect(r, core::Mouse::Position())) {
        if (isAnyBtnDown()) {
            return Event::MouseDown;
        } else {
            return Event::MouseMotion;
        }
    } else {
        return Event::NoEvent;
    }
}

ColorStyle& detectColorStyle(Event event, Style* userStyle) {
    if (userStyle) {
        if (event == Event::MouseMotion) {
            return userStyle->hoverColors;
        } else if (event == Event::MouseDown) {
            return userStyle->pressColors;
        }
        return userStyle->normalColors;
    } else {
        if (event == Event::MouseMotion) {
            return Context.style.hoverColors;
        } else if (event == Event::MouseDown) {
            return Context.style.pressColors;
        }
        return Context.style.normalColors;
    }
}

bool Button(bool& show, const char* text, core::Rect* rect, Align align, Style* style) {
    if (!show) {
        return false;
    }
    auto& renderer = core::VideoMgr::GetMainVideo()->renderer;
    auto* drawRect = rect ? rect : (Context.presentRect ? &Context.presentRect.value() : nullptr);
    if (drawRect) {
        auto size = Context.font->GetSize(text);
        core::Rect textRect(drawRect->x, drawRect->y, size.w, size.h);
        AlignRect(Align::AlignCenter, *drawRect, textRect);
        AlignRect(align,  *drawRect, textRect);

        Event evt = detectEvent(*drawRect);
        auto& colorStyle = detectColorStyle(evt, style);

        renderer->DrawRect(*drawRect,
                           colorStyle.bgColor,
                           colorStyle.borderColor);

        auto texture = renderer->GenerateText(*Context.font, text,
                                              colorStyle.fontColor);

        auto image = core::Image::Create(*texture);
        core::Transform transform;
        transform.SetPos(core::Vec2(textRect.x, textRect.y));
        renderer->DrawImage(image, transform);

        return evt == Event::MouseDown;
    }
    return false;
}

}
