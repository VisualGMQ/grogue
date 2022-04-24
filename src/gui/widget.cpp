#include "gui/widget.hpp"

namespace grogue::gui {

bool isAnyBtnDown() {
    return core::Mouse::IsPressed(core::MouseButton::Left)||
           core::Mouse::IsPressed(core::MouseButton::Right)||
           core::Mouse::IsPressed(core::MouseButton::Middle);
}

bool isAnyBtnUp() {
    return core::Mouse::IsReleased(core::MouseButton::Left)||
           core::Mouse::IsReleased(core::MouseButton::Right)||
           core::Mouse::IsReleased(core::MouseButton::Middle);
}

bool Button(bool& show, const char* text, grogue::core::Rect* rect) {
    if (!show) {
        return false;
    }
    auto& renderer = core::VideoMgr::GetMainVideo()->renderer;
    auto* drawRect = rect ? rect : (Context.presentRect ? &Context.presentRect.value() : nullptr);
    if (drawRect) {
        renderer->DrawRect(*drawRect,
                           Context.colorScheme.borderColor,
                           Context.colorScheme.bgColor); 
        auto texture = renderer->GenerateText(*Context.font, text,
                                              Context.colorScheme.fontColor);
        core::Renderer::TextureRenderInfo info(texture.get());
        info.SetPos(core::Vec2(rect->x, rect->y));
        renderer->DrawTexture(info);

        if (core::IsPointInRect(*rect, core::Mouse::Position()) &&
            isAnyBtnDown()) {
        return true;
        }
    }
    return false;
}

}
