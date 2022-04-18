#include "core/engine.hpp"

namespace grogue::core {

bool Engine::shouldQuit_ = false;

void Engine::Init(const std::string& title,
                  uint32_t w, uint32_t h,
                  bool resizable) {
    Log::Init(LogLevel::Debug);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        LOG_CRITICAL("SDL initialized failed");
        exit(CORE_SDL_INIT_FAILED);
    }
    LOG_INFO("SDL initialized");

    if (IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) == 0) {
        LOG_CRITICAL("SDL_image initialized failed!");
        SDL_Quit();
        exit(CORE_IMG_INIT_FAILED);
    }
    LOG_INFO("SDL_image initialized");

    if (TTF_Init() == -1) {
        LOG_CRITICAL("SDL_ttf initialized failed");
        exit(CORE_TTF_INIT_FAILED);
    }
    LOG_INFO("SDL_ttf initialized");

    VideoMgr::Init(title.c_str(), w, h, resizable);
    LOG_INFO("VideoMgr initialized");
}

void Engine::Exit() {
    shouldQuit_ = true;
}

void Engine::cleanUp() {
    VideoMgr::Quit();
    LOG_INFO("VideoMgr quited");

    TTF_Quit();
    LOG_INFO("SDL_ttf quited");

    IMG_Quit();
    LOG_INFO("SDL_image quited");

    SDL_Quit();
    LOG_INFO("SDL quited");

}

bool Engine::ShouldQuit() {
    return shouldQuit_;
}

}
