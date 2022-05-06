#pragma once

#include "pch.hpp"
#include "mathf.hpp"

namespace grogue::core {

using ID = std::uint32_t;

class Window final {
public:
    friend class Renderer;
    friend class Engine;

    Window(const char* title,
           uint32_t width, uint32_t height,
           bool resizable = false);
    Window(const Window&) = delete;
    ~Window();

    void Show();
    void Hide();
    bool IsShown() const;
    Size GetSize() const;
    Size GetOriginSize() const;

    ID GetID() const;

    Window& operator=(const Window&) = delete;

private:
    SDL_Window* window_;
    Size originSize_;
};

}
