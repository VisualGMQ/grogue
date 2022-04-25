#include "gui/init.hpp"

namespace grogue::gui {

GuiContext Context;

void Init(grogue::core::Font* font) {
    Context.font = font;
    Context.presentRect = std::nullopt;
    SetLightColorscheme();
}

void SetLightColorscheme() {
    Context.style.normalColors.bgColor = core::Color(10, 50, 10);
    Context.style.normalColors.fontColor = core::Color(255, 255, 255);
    Context.style.normalColors.borderColor = core::Color(200, 200, 200);

    Context.style.hoverColors.bgColor = core::Color(10, 30, 10);
    Context.style.hoverColors.fontColor = core::Color(255, 255, 255);
    Context.style.hoverColors.borderColor = core::Color(200, 200, 200);

    Context.style.pressColors.bgColor = core::Color(10, 10, 10);
    Context.style.pressColors.fontColor = core::Color(255, 255, 255);
    Context.style.pressColors.borderColor = core::Color(200, 200, 200);
}

void Quit() {}

}
