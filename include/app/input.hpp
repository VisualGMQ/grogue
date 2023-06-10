#pragma once

#include "core/pch.hpp"
#include "core/math.hpp"
#include "core/ecs.hpp"
#include "app/fwd.hpp"

template <typename T>
struct Button {
    T btn;
    bool isPress = false;
    bool lastState = false;

    bool IsPressed() const { return !lastState && isPress; }
    bool IsReleased() const { return lastState && !isPress; }
    bool IsPressing() const { return lastState && isPress; }
    bool IsReleasing() const { return !lastState && !isPress; }
};

using KeyButton = Button<SDL_Scancode>;

class LUA_BIND_RESOURCE Keyboard {
public:
    static void UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&);

    Keyboard();
    const KeyButton& Key(SDL_Scancode key) const { return buttons_[key]; }

private:
    KeyButton buttons_[SDL_NUM_SCANCODES];

    void updateOneKey(const SDL_KeyboardEvent& event);
};

using MouseButton = Button<uint8_t>;

class LUA_BIND_RESOURCE Mouse {
public:
    static void UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&);

    math::Vector2 Position() const { return position_; }
    math::Vector2 Offset() const { return offset_; }
    const MouseButton& LeftBtn() const { return buttons_[0]; }
    const MouseButton& MiddleBtn() const { return buttons_[1]; }
    const MouseButton& RightBtn() const { return buttons_[2]; }

private:
    math::Vector2 position_;
    math::Vector2 offset_;
    MouseButton buttons_[3] = {MouseButton{SDL_BUTTON_LEFT},
                                MouseButton{SDL_BUTTON_MIDDLE},
                                MouseButton{SDL_BUTTON_RIGHT}};
    
    void updateOneBtn(const SDL_MouseButtonEvent&);
};