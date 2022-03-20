#pragma once

#include "pch.hpp"

class Window final {
public:
    friend class WindowManager;

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
};

class WindowManager {
public:
private:
};
