#include "app/input.hpp"

Keyboard::Keyboard() {
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        buttons_[i].btn = static_cast<SDL_Scancode>(i);
    }
}

void Keyboard::UpdateSystem(ecs::Commands&, ecs::Queryer, ecs::Resources resources, ecs::Events& events) {
    if (!resources.Has<Keyboard>()) {
        return ;
    }

    auto& keyboard = resources.Get<Keyboard>();
    auto reader = events.Reader<SDL_KeyboardEvent>();

    if (reader.Has()) {
        auto& event = reader.Read();
        auto& btn = keyboard.buttons_[event.keysym.scancode];
        btn.lastState = btn.isPress;
        if (reader.Has()) {
            for (auto& btn : keyboard.buttons_) {
                btn.isPress = false;
            }
        }
        if (event.type == SDL_KEYDOWN) {
            btn.isPress = true;
        }
    }
}

void Mouse::UpdateSystem(ecs::Commands&, ecs::Queryer, ecs::Resources resources, ecs::Events& events) {
    if (!resources.Has<Mouse>()) {
        return ;
    }

    auto& mouse = resources.Get<Mouse>();
    auto motionReader = events.Reader<SDL_MouseMotionEvent>();
    if (motionReader.Has()) {
        auto& motion = motionReader.Read();
        mouse.position_.x = motion.x;
        mouse.position_.y = motion.y;
    }

    auto btnReader = events.Reader<SDL_MouseButtonEvent>();

    if (btnReader.Has()) {
        auto& button = btnReader.Read();
        auto& btn = mouse.buttons_[button.button - 1];
        btn.lastState = btn.isPress;
        for (auto& btn : mouse.buttons_) {
            btn.isPress = false;
        }
        if (button.type == SDL_MOUSEBUTTONDOWN) {
            btn.isPress = true;
        }
    }
}