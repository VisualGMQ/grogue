#pragma once

#include "pch.hpp"
#include "window.hpp"
#include "renderer.hpp"

namespace grogue::core {

using ID = uint32_t;

class VideoMgr {
public:
    struct Video {
        std::unique_ptr<Window> window;
        std::unique_ptr<Renderer> renderer;
        ID id;
    };

    static void Init(const char* title,
                     std::uint32_t width, std::uint32_t height,
                     bool resizable);
    static void Quit();
    static Video& GetMainVideo();
    static Video& GetVideo(ID id);
    static Video& CreateVideo(const char* title,
                             std::uint32_t width, std::uint32_t height,
                             bool resizable);
    static void Present();

private:
    static std::unordered_map<ID, Video> videos_;
    static uint32_t curId;
};

}
