#pragma once

#include "pch.hpp"

namespace grogue::core {

class Window final {
public:
    friend class Renderer;

    Window(const char* title,
           uint32_t width, uint32_t height,
           bool resizable = false);
    Window(const Window&) = delete;
    ~Window();

    Window& operator=(const Window&) = delete;

private:
    SDL_Window* window_;
};

}
