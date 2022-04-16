#include "core/window.hpp"

namespace grogue::core {
 
Window::Window(const char* title,
               uint32_t width, uint32_t height,
               bool resizable) {
    window_ = SDL_CreateWindow(title,
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               (int)width, (int)height,
                               SDL_WINDOW_SHOWN |
                               (resizable ? SDL_WINDOW_RESIZABLE : 0));
}

void Window::Show() {
    SDL_ShowWindow(window_);
}

void Window::Hide() {
    SDL_HideWindow(window_);
}

bool Window::IsShown() const {
    return SDL_GetWindowFlags(window_) & SDL_WINDOW_SHOWN;
}

ID Window::GetID() const {
    return SDL_GetWindowID(window_);
}

Window::~Window() {
    SDL_DestroyWindow(window_);
}

}
