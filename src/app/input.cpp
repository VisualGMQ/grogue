#include "app/input.hpp"

Keyboard::Keyboard() {
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        buttons_[i].btn = static_cast<SDL_Scancode>(i);
    }
}

void Keyboard::UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources resources, ecs::Events& events) {
    if (!resources.Has<Keyboard>()) {
        return ;
    }

    auto& keyboard = resources.Get<Keyboard>();
    auto reader = events.Reader<KeyboardEvents>();

    for (auto& btn : keyboard.buttons_) {
        btn.lastState = btn.isPress;
    }

    if (reader.Has()) {
        auto& eventList = reader.Read();
        for (auto& event : eventList.events) {
            keyboard.updateOneKey(event);
        }
    }
}

void Keyboard::updateOneKey(const SDL_KeyboardEvent& event) {
    auto& btn = buttons_[event.keysym.scancode];
    btn.lastState = btn.isPress;
    if (event.type == SDL_KEYDOWN) {
        btn.isPress = true;
    }
    if (event.type == SDL_KEYUP) {
        btn.isPress = false;
    }
}

void Mouse::UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources resources, ecs::Events& events) {
    if (!resources.Has<Mouse>()) {
        return ;
    }

    auto& mouse = resources.Get<Mouse>();
    auto motionReader = events.Reader<SDL_MouseMotionEvent>();
    if (motionReader.Has()) {
        auto& motion = motionReader.Read();
        mouse.position_.x = motion.x;
        mouse.position_.y = motion.y;
        mouse.offset_.x = motion.xrel;
        mouse.offset_.y = motion.yrel;
    }

    auto reader = events.Reader<MouseBtnEvents>();

    for (auto& btn : mouse.buttons_) {
        btn.lastState = btn.isPress;
    }

    if (reader.Has()) {
        auto& eventList = reader.Read();
        for (auto& event : eventList.events) {
            mouse.updateOneBtn(event);
        }
    }
}

void Mouse::updateOneBtn(const SDL_MouseButtonEvent& event) {
    auto& btn = buttons_[event.button - 1];
    btn.lastState = btn.isPress;
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        btn.isPress = true;
    }
    if (event.type == SDL_MOUSEBUTTONUP) {
        btn.isPress = false;
    }
}