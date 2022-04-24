#pragma once

#include "core/pch.hpp"

namespace grogue::gui {

struct ColorScheme {
    std::optional<grogue::core::Color> borderColor;
    grogue::core::Color bgColor;
    grogue::core::Color fontColor;
};

}
