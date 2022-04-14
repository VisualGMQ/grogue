#include "core/init.hpp"

namespace grogue::core {

void InitSystem(const char* title,
                std::uint32_t w, std::uint32_t h,
                bool resizable) {
    SDL_Init(SDL_INIT_EVERYTHING);
    spdlog::info("SDL initialized");

    IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
    spdlog::info("SDL_image initialized");

    TTF_Init();
    spdlog::info("SDL_ttf initialized");

    VideoMgr::Init(title, w, h, resizable);
    spdlog::info("VideoMgr initialized");
}

void QuitSystem() {
    VideoMgr::Quit();
    spdlog::info("VideoMgr quited");

    TTF_Quit();
    spdlog::info("SDL_ttf quited");

    IMG_Quit();
    spdlog::info("SDL_image quited");

    SDL_Quit();
    spdlog::info("SDL quited");
}

}
