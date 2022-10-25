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

class DLLEXPORT Mouse final {
public:
    Mouse() = delete;

    DLLEXPORT static void Hide();
    DLLEXPORT static void Show();
    DLLEXPORT static bool IsShowing();
    DLLEXPORT static void ToggleShow();
    DLLEXPORT static void Capture(bool);
    DLLEXPORT static void RelativeMode(bool);
    DLLEXPORT static bool IsRelativeMode();
    DLLEXPORT static void ToggleRelativeMode();
    DLLEXPORT static void SetCursor(const Cursor&);
    DLLEXPORT static void SetSystemCursor();

private:
    static bool isShow_;
};

}