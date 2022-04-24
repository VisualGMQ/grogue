#pragma once

#include "pch.hpp"
#include "core/mathf.hpp"
#include "core/window.hpp"
#include "core/video.hpp"

namespace grogue::core {

enum class MouseButton {
    Left = SDL_BUTTON_LEFT,
    Right = SDL_BUTTON_RIGHT,
    Middle = SDL_BUTTON_MIDDLE,
    X1 = SDL_BUTTON_X1,
    X2 = SDL_BUTTON_X2,
};

class Mouse final {
public:
    Mouse() = delete;
    static void Init();

    /* get the real position in window */
    static inline Vec2 RealPosition() { return pos_; }
    /* get the scaled position in window(position scales with window size) */
    static Vec2 Position();
    static inline bool IsPressed(MouseButton btn) {
        return buttonStates_[static_cast<Uint8>(btn) - 1] &&
               !prevButtonStates_[static_cast<Uint8>(btn) - 1];
    }
    static inline bool IsPressing(MouseButton btn) {
        return buttonStates_[static_cast<Uint8>(btn) - 1] &&
               prevButtonStates_[static_cast<Uint8>(btn) - 1];

    }
    static inline bool IsReleased(MouseButton btn) {
        return !buttonStates_[static_cast<Uint8>(btn) - 1] &&
               prevButtonStates_[static_cast<Uint8>(btn) - 1];

    }
    static inline bool IsReleasing(MouseButton btn) {
        return !buttonStates_[static_cast<Uint8>(btn) - 1] &&
               !prevButtonStates_[static_cast<Uint8>(btn) - 1];
    }

    static void AcceptEvent(const SDL_Event&);
    static void SwapButtonStates();
    static void SetFocusWindow(Window*);

private:
    static core::Vec2 pos_;
    static core::Vec2 offset_;
    static bool buttonStates_[5];
    static bool prevButtonStates_[5];
    static Window* focusedWindow_;
};

}
