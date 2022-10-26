#pragma once

#include "engine/core/pch.hpp"
#include "engine/input/keycode.hpp"
#include "engine/core/vmath.hpp"
#include "engine/core/video.hpp"
#include "engine/input/keycode.hpp"

namespace engine {

#define MOUSE_BTN_NUM 6

class Input final {
public:
    Input() = delete;

    static void Init();
    static void Quit();

    static bool IsKeyPressed(Key);
    static bool IsKeyPressing(Key);
    static bool IsKeyReleased(Key);
    static bool IsKeyReleasing(Key);

    static bool IsButtonPressed(MouseButton);
    static bool IsButtonPressing(MouseButton);
    static bool IsButtonReleased(MouseButton);
    static bool IsButtonReleasing(MouseButton);

    static Vec2 MousePoition();
    static const Vec2& MouseRealPoition();
    static const Vec2& MouseRelative();

    static void UpdateStates();
    static void UpdateKeyState(Key key, bool isPress); 
    static void UpdateMouseBtnState(Uint8 btn, bool isPress); 
    static void UpdateMousePosition(const Vec2& position, const Vec2& relpos); 

private:
    static bool oldKeyState_[SDL_NUM_SCANCODES];
    static bool keyState_[SDL_NUM_SCANCODES];
    static bool oldBtnState_[MOUSE_BTN_NUM];
    static bool btnState_[MOUSE_BTN_NUM];
    static Vec2 position_;
    static Vec2 offset_;
};

}
