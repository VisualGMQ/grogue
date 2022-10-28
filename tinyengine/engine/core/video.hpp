#pragma once

#include "engine/core/pch.hpp"

namespace engine {

class Video final {
public:
    Video() = delete;

    static void Init();
    static void Quit();
    
    static void SwapBuffers();
    static bool ShouldClose();
    static void Close();

    static void* GetWindow() { return window_; }

    static Size GetWindowSize() {
        if (window_) {
            int w, h;
            SDL_GetWindowSize(window_, &w, &h);
            return Size(w, h);
        } else {
            return {};
        }
    }
    static const Size& GetInitSize() { return initSize_; }

    static SDL_Renderer* GetRenderer() { return renderer_; }

    static void RegistVideo(SDL_Window* window, SDL_Renderer* renderer);

private:
    static SDL_Window* window_;
    static SDL_Renderer* renderer_;
    static bool shouldClose_;
    static Vec2 initSize_;
};

}
