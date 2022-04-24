#pragma once

namespace grogue::gui {

enum Event {
    GUI_NoEvent     = 0,
    GUI_MouseUp     = 0x00000002,
    GUI_MouseDown   = 0x00000004,
    GUI_MouseMotion = 0x00000008,
    GUI_KeyDown     = 0x00000010,
    GUI_KeyUp       = 0x00000020,
};

}
