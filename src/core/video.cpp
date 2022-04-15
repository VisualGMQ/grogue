#include "core/video.hpp"

namespace grogue::core {

std::unordered_map<ID, VideoMgr::Video> VideoMgr::videos_;
uint32_t VideoMgr::curId = 0;

VideoMgr::Video& VideoMgr::GetMainVideo() {
    return videos_[0];
}

VideoMgr::Video& VideoMgr::GetVideo(ID id) {
    auto it = videos_.find(id);
    if (it != videos_.end()) {
        return it->second;
    } else {
        spdlog::error("don't exists Video (ID {})", id);
        return videos_[id];
    }
}

VideoMgr::Video& VideoMgr::CreateVideo(const char* title,
                                      std::uint32_t width, std::uint32_t height,
                                      bool resizable) {
    Video video;
    video.window.reset(new Window(title, width, height, resizable));
    if (!video.window) {
        spdlog::error("can't create window");
    } else {
        spdlog::info("create window OK");
    }

    video.renderer.reset(new Renderer(*video.window));
    if (!video.renderer) {
        spdlog::error("can't create SDL renderer");
    } else {
        spdlog::info("create renderer OK");
    }

    auto result = videos_.emplace(curId, std::move(video));
    if (!result.second) {
        spdlog::error("create Video failed!");
    }
    curId++;
    return result.first->second;
}

void VideoMgr::Init(const char* title,
                    std::uint32_t width, std::uint32_t height,
                    bool resizable) {
    CreateVideo(title, width, height, resizable);
}

void VideoMgr::Quit() {
    for (auto& video : videos_) {
        video.second.window.reset();
        video.second.renderer.reset();
    }
}

void VideoMgr::Present() {
    for (auto& video : videos_) {
        video.second.renderer->Present();
    }
}

}
