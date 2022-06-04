#pragma once

#include "pch.hpp"
#include "SDL_keycode.h"

namespace grogue::core {

class Keyboard final {
public:
    Keyboard() = delete;

    static void Init();
    static void SwapKeyState();
    static void AccpetEvent(const SDL_Event& e);

    static inline bool IsPressing(SDL_Scancode code)  { return states_[code] && prevStates_[code]; }
    static inline bool IsPressed(SDL_Scancode code)   { return states_[code] && !prevStates_[code]; }
    static inline bool IsReleasing(SDL_Scancode code) { return !states_[code] && !prevStates_[code]; }
    static inline bool IsReleased(SDL_Scancode code)  { return !states_[code] && prevStates_[code]; }
    static inline bool IsPress(SDL_Scancode code)     { return IsPressed(code) || IsPressing(code); }
    static inline bool IsRelease(SDL_Scancode code)   { return IsReleased(code) || IsReleasing(code); }

private:
    static bool states_[SDL_NUM_SCANCODES];
    static bool prevStates_[SDL_NUM_SCANCODES];
};

}
