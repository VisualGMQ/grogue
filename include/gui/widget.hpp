#pragma once

#include "gui/init.hpp"
#include "core/video.hpp"
#include "core/mouse.hpp"

namespace grogue::gui {

bool Button(bool& show, const char* text, grogue::core::Rect* rect = nullptr);

}
