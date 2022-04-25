#pragma once

#include "core/pch.hpp"
#include "core/font.hpp"
#include "event.hpp"
#include "style.hpp"

namespace grogue::gui {

struct GuiContext {
    grogue::core::Font* font;
    std::optional<grogue::core::Rect> presentRect;
    Style style;
};

extern GuiContext Context;

void SetLightColorscheme();

void Init(grogue::core::Font* font);
void Quit();

}
