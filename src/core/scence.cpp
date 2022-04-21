#include "core/scence.hpp"

namespace grogue::core {

Scence* ScenceMgr::curScence_ = nullptr;
Scence* ScenceMgr::oldScence_ = nullptr;

Scence::Scence(std::string_view name): name_(name) {}

void Scence::OnUpdate() {
    for (auto& layer : layers_) {
        if (layer->IsWorking()) {
            layer->Update();
        }
    }
}

void Scence::OnRender() {
    for (auto& layer : layers_) {
        if (layer->IsWorking()) {
            layer->Render();
        }
    }
}

void Scence::OnEventHandle(const SDL_Event& event) {
    EventDispatcher dispatcher(layers_);

    dispatcher.Dispatch<SDL_KEYDOWN>(event.key);
    dispatcher.Dispatch<SDL_KEYUP>(event.key);
    dispatcher.Dispatch<SDL_MOUSEMOTION>(event.motion);
    dispatcher.Dispatch<SDL_MOUSEBUTTONDOWN>(event.button);
    dispatcher.Dispatch<SDL_MOUSEBUTTONUP>(event.button);
}


Storage<std::string_view, std::unique_ptr<Scence>> ScenceMgr::storage_;

void ScenceMgr::Init(Scence* scence) {
    curScence_ = scence;
    if (curScence_) {
        curScence_->OnInit();
        for (auto& layer : scence->layers_) {
            layer->OnInit();
        }
    } else {
        LOG_ERROR("first runnig scence can't be nullptr");
    }
}

void ScenceMgr::Quit() {
    if (curScence_) {
        curScence_->OnQuit();
        for (auto& layer : curScence_->layers_) {
            layer->OnQuit();
        }
    }
}

void ScenceMgr::SwitchScence(std::string_view name) {
    oldScence_ = curScence_;
    Scence* scence = GetScence(name);
    if (scence) {
        scence->OnInit();
    } else {
        LOG_ERROR("Scence name not exists", name);
    }
    curScence_ = scence;
}

Scence* ScenceMgr::GetCurrentScence() {
    return curScence_;
}

void ScenceMgr::CleanUpOldScence() {
    if (oldScence_) {
        oldScence_->OnQuit();
        oldScence_ = nullptr;
    }
}

Scence* ScenceMgr::GetScence(std::string_view name) {
    auto scence = storage_.Find(name);
    if (!scence) {
        return nullptr;
    } else {
        return scence->get();
    }
}

}
