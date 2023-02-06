#pragma once

#include "SDL.h"
#include "core/math.hpp"
#include "core/assert.hpp"

class Window final {
public:
    friend class Renderer;

    Window(const std::string& title, uint32_t w, uint32_t h);
    Window(const Window&) = delete;
    Window(Window&&);
    ~Window();

    Window& operator=(Window&&);
    Window& operator=(const Window&) = delete;

    math::Vector2 GetSize() const;

private:
    SDL_Window* window_;
};