#include "app/window.hpp"

Window::Window(const std::string& title, uint32_t w, uint32_t h) {
    window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    Assert(window_ != nullptr, "window create failed");
}

math::Vector2 Window::GetSize() const {
    int w, h;
    SDL_GetWindowSize(window_, &w, &h);
    return math::Vector2(w, h);
}

Window::Window(Window&& window) {
    swap(window, *this);
}

Window& Window::operator=(Window&& window) {
    if (&window != this) {
        swap(window, *this);
        window.window_ = nullptr;
    }
    return *this;
}

Window::~Window() {
    SDL_DestroyWindow(window_);
}
