#include "core/init.hpp"

namespace grogue::core {

void InitSystem(const char* title,
                std::uint32_t w, std::uint32_t h,
                bool resizable) {
    Log::Init(LogLevel::Debug);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        LOG_CRITICAL("SDL initialized failed");
        exit(SDL_INIT_FAILED);
    }
    LOG_INFO("SDL initialized");

    if (IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) == 0) {
        LOG_CRITICAL("SDL_image initialized failed!");
        SDL_Quit();
        exit(IMG_INIT_FAILED);
    }
    LOG_INFO("SDL_image initialized");

    if (TTF_Init() == -1) {
        LOG_CRITICAL("SDL_ttf initialized failed");
        exit(TTF_INIT_FAILED);
    }
    LOG_INFO("SDL_ttf initialized");

    VideoMgr::Init(title, w, h, resizable);
    LOG_INFO("VideoMgr initialized");
}

void QuitSystem() {
    VideoMgr::Quit();
    LOG_INFO("VideoMgr quited");

    TTF_Quit();
    LOG_INFO("SDL_ttf quited");

    IMG_Quit();
    LOG_INFO("SDL_image quited");

    SDL_Quit();
    LOG_INFO("SDL quited");
}

}
