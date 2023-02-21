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

class Keyboard {
public:
    static void UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&);

    Keyboard();
    const Button<SDL_Scancode>& Key(SDL_Scancode key) const { return buttons_[key]; }

private:
    Button<SDL_Scancode> buttons_[SDL_NUM_SCANCODES];

    void updateOneKey(const SDL_KeyboardEvent& event);
};

class Mouse {
public:
    static void UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&);

    math::Vector2 Position() const { return position_; }
    math::Vector2 Offset() const { return offset_; }
    const Button<uint8_t>& LeftBtn() const { return buttons_[0]; }
    const Button<uint8_t>& MiddleBtn() const { return buttons_[1]; }
    const Button<uint8_t>& RightBtn() const { return buttons_[2]; }

private:
    math::Vector2 position_;
    math::Vector2 offset_;
    Button<uint8_t> buttons_[3] = {Button<uint8_t>{SDL_BUTTON_LEFT},
                                   Button<uint8_t>{SDL_BUTTON_MIDDLE},
                                   Button<uint8_t>{SDL_BUTTON_RIGHT}};
    
    void updateOneBtn(const SDL_MouseButtonEvent&);
};