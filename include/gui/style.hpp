#pragma once

#include "core/mathf.hpp"
#include "core/pch.hpp"

namespace grogue::gui {

struct ColorStyle {
    std::optional<core::Color> borderColor;
    core::Color bgColor;
    core::Color fontColor;
};

struct Style {
    ColorStyle normalColors;
    ColorStyle hoverColors;
    ColorStyle pressColors;
};

}
