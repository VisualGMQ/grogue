#pragma once

#include "engine/core/pch.hpp"
#include "engine/core/video.hpp"

namespace engine {

class Mouse;

class Cursor final {
public:
    friend class Mouse;
    Cursor(std::string_view filename, int x, int y, const Color& keyColor);
    Cursor(SDL_SystemCursor id);
    ~Cursor();

private:
    SDL_Cursor* cursor_;
};

class Mouse final {
public:
    Mouse() = delete;

    static void Hide();
    static void Show();
    static bool IsShowing();
    static void ToggleShow();
    static void Capture(bool);
    static void RelativeMode(bool);
    static bool IsRelativeMode();
    static void ToggleRelativeMode();
    static void SetCursor(const Cursor&);
    static void SetSystemCursor();

private:
    static bool isShow_;
};

}