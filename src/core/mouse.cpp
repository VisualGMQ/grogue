#include "core/mouse.hpp"

namespace grogue::core  {

core::Vec2 Mouse::pos_;
core::Vec2 Mouse::offset_;
bool Mouse::buttonStates_[5];
bool Mouse::prevButtonStates_[5];
Window* Mouse::focusedWindow_ = nullptr;

void Mouse::Init() {
    memset(buttonStates_, 0, sizeof(buttonStates_));
    memset(prevButtonStates_, 0, sizeof(prevButtonStates_));
    SetFocusWindow(VideoMgr::GetMainVideo()->window.get());
}

void Mouse::SwapButtonStates() {
    memcpy(prevButtonStates_, buttonStates_, sizeof(buttonStates_));
}

Vec2 Mouse::Position() {
    Vector<float, 2> scale(focusedWindow_->GetSize().x / focusedWindow_->GetOriginSize().x,
                            focusedWindow_->GetSize().y / focusedWindow_->GetOriginSize().y,);
    return Mouse::pos_ * scale;
}

void Mouse::AcceptEvent(const SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
        pos_.x = e.motion.x;
        pos_.y = e.motion.y;
        offset_.x = e.motion.xrel;
        offset_.y = e.motion.yrel;
    }
}

void Mouse::SetFocusWindow(Window* window) {
    focusedWindow_ = window;
}

}
