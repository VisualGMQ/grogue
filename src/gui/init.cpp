#include "gui/init.hpp"

namespace grogue::gui {

GuiContext Context;

void Init(grogue::core::Font* font) {
    Context.font = font;
    Context.presentRect = std::nullopt;
    Context.event = Event::GUI_NoEvent;
    SetLightColorscheme();
}

void SetLightColorscheme() {
    Context.colorScheme.bgColor = core::Color(10, 50, 10);
    Context.colorScheme.fontColor = core::Color(255, 255, 255);
    Context.colorScheme.borderColor = core::Color(200, 200, 200);
}

void Quit() {}

}
