#pragma once

#include "pch.hpp"

namespace grogue::core {

class VideoMgr {
public:
    struct Video {
        SDL_Window* window;
        SDL_Renderer* renderer;
    };

    static void Init(const char* title,
                     std::uint32_t width, std::uint32_t height,
                     bool resizable);
    static void Quit();
    static Video& GetVideo();

private:
    static Video video_;
};

}
