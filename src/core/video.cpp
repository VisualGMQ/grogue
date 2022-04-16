#include "core/video.hpp"

namespace grogue::core {

std::optional<ID> VideoMgr::mainWindowID_ = std::nullopt;

std::unordered_map<ID, VideoMgr::Video> VideoMgr::videos_;

VideoMgr::Video& VideoMgr::GetMainVideo() {
    if (!mainWindowID_) {
        spdlog::error("don't have main window");
    }
    return videos_[mainWindowID_.value()];
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

    if (!mainWindowID_) {
        mainWindowID_ = video.window->GetID();
    }

    auto result = videos_.emplace(video.window->GetID(), std::move(video));
    if (!result.second) {
        spdlog::error("create Video failed!");
    }
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

VideoMgr::Video& VideoMgr::FindByID(ID id) {
    return videos_[id];
}

void VideoMgr::Present() {
    for (auto& video : videos_) {
        video.second.renderer->Present();
    }
}

}
