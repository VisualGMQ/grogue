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

Window::~Window() {
    SDL_DestroyWindow(window_);
}

}
