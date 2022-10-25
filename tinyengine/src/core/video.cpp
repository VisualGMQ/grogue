#include "engine/core/video.hpp"

namespace engine {

SDL_Window* Video::window_ = nullptr;
SDL_Renderer* Video::renderer_ = nullptr;
bool Video::shouldClose_ = false;
Vec2 Video::initSize_;

void Video::Init(std::string_view title, int w, int h, bool resizable) {
    initSize_.Set(w, h);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Loge("SDL init failed: {}", SDL_GetError());
        exit(1);
    }

    uint32_t flags = SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI;
    if (resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }
    window_ = SDL_CreateWindow(title.data(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              w, h,
                              flags);

    if (!window_) {
        SDL_Quit();
        Loge("window create failed");
        exit(2);
    }

    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    if (!renderer_) {
        SDL_DestroyWindow(window_);
        SDL_Quit();
        Loge("sdl renderer create failed");
        exit(2);
    }
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

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

}
