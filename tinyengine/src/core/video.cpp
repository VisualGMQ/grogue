#include "engine/core/video.hpp"

namespace engine {

SDL_Window* Video::window_ = nullptr;
SDL_Renderer* Video::renderer_ = nullptr;
bool Video::shouldClose_ = false;
Vec2 Video::initSize_;

void Video::Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Loge("SDL init failed: {}", SDL_GetError());
        exit(1);
    }

    shouldClose_ = false;
}

void Video::Quit() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Video::SwapBuffers() {
    SDL_RenderPresent(renderer_);
}

bool Video::ShouldClose() {
    return shouldClose_;
}

void Video::Close() {
    shouldClose_ = true;
}


void Video::RegistVideo(SDL_Window* window, SDL_Renderer* renderer) {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
    }
    renderer_ = renderer;
    if (window_) {
        SDL_DestroyWindow(window_);
    }
    window_ = window;

    initSize_ = GetWindowSize();
}

}
