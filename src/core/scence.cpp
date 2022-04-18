#include "core/scence.hpp"

namespace grogue::core {

std::unordered_map<std::string_view, std::unique_ptr<Scence>> ScenceMgr::scences_;
Scence* ScenceMgr::curScence_ = nullptr;
Scence* ScenceMgr::oldScence_ = nullptr;

Scence::Scence(std::string_view name): name_(name) {}

void ScenceMgr::Init(Scence* scence) {
    curScence_ = scence;
    if (curScence_) {
        curScence_->OnInit();
    } else {
        LOG_ERROR("first runnig scence can't be nullptr");
    }
}

void ScenceMgr::Quit() {
    if (curScence_) {
        curScence_->OnQuit();
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
    auto it = scences_.find(name);
    if (it != scences_.end()) {
        return it->second.get();
    } else {
        return nullptr;
    }
}

}
