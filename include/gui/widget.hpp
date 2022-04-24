#pragma once

#include "gui/init.hpp"
#include "core/video.hpp"
#include "core/mouse.hpp"

namespace grogue::gui {

enum Align {
    AlignLeft    = 0x01,
    AlignRight   = 0x02,
    AlignTop     = 0x04,
    AlignBottom  = 0x08,
    AlignXCenter = 0x10,
    AlignYCenter = 0x20,

    AlignCenter = AlignXCenter | AlignYCenter,
};


bool Button(bool& show, const char* text, grogue::core::Rect* = nullptr, Align = Align::AlignCenter);

}
