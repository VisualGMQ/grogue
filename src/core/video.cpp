#include "core/video.hpp"

namespace grogue::core {

Storage<ID, Video> VideoMgr::storage_;
std::optional<ID> VideoMgr::mainWindowID_ = std::nullopt;

Video* VideoMgr::GetMainVideo() {
    if (!mainWindowID_) {
        LOG_ERROR("don't have main window");
    }
    return storage_.Find(mainWindowID_.value());
}

Video* VideoMgr::GetVideo(ID id) {
    auto video = storage_.Find(id);
    if (video) {
        return video;
    } else {
        LOG_ERROR("don't exists Video (ID %lu)", id);
        return nullptr;
    }
}

Video* VideoMgr::CreateVideo(const char* title,
                             std::uint32_t width, std::uint32_t height,
                             bool resizable) {
    Video video;
    video.window.reset(new Window(title, width, height, resizable));
    if (!video.window) {
        LOG_ERROR("can't create window");
    } else {
        LOG_INFO("create window OK");
    }

    video.renderer.reset(new Renderer(*video.window));
    if (!video.renderer) {
        LOG_ERROR("can't create SDL renderer");
    } else {
        LOG_INFO("create renderer OK");
    }

    if (!mainWindowID_) {
        mainWindowID_ = video.window->GetID();
    }

    video.id = video.window->GetID();
    return &storage_.Create(video.window->GetID(),
                            std::move(video));
}

void VideoMgr::Init(const char* title,
                    std::uint32_t width, std::uint32_t height,
                    bool resizable) {
    CreateVideo(title, width, height, resizable);
}

void VideoMgr::Quit() {
    for (auto& video : storage_) {
        video.second.window.reset();
        video.second.renderer.reset();
    }
}

Video* VideoMgr::FindByID(ID id) {
    return storage_.Find(id);
}

void VideoMgr::Present() {
    for (auto& video : storage_) {
        video.second.renderer->Present();
    }
}

void VideoMgr::ClearScreen() {
    for (auto& video : storage_) {
        video.second.renderer->Clear(Color(0, 0, 0));
    }
}

}
