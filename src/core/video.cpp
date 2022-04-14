#include "core/video.hpp"

namespace grogue::core {

VideoMgr::Video VideoMgr::video_{nullptr, nullptr};

VideoMgr::Video& VideoMgr::GetVideo() {
    return video_;
}

void VideoMgr::Init(const char* title,
                    std::uint32_t width, std::uint32_t height,
                    bool resizable) {
    video_.window = SDL_CreateWindow(title,
                                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                     width, height,
                                     SDL_WINDOW_SHOWN | (resizable ? SDL_WINDOW_RESIZABLE : 0));
    if (!video_.window) {
        spdlog::error("can't create window");
    } else {
        spdlog::info("create window OK");
    }

    video_.renderer = SDL_CreateRenderer(video_.window, -1, 0);
    if (!video_.renderer) {
        spdlog::error("can't create SDL renderer");
    } else {
        spdlog::info("create renderer OK");
    }
}

void VideoMgr::Quit() {
    SDL_DestroyRenderer(video_.renderer);
    SDL_DestroyWindow(video_.window);
}

}
