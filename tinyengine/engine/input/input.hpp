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

    DLLEXPORT static void Init();
    DLLEXPORT static void Quit();

    DLLEXPORT static bool IsKeyPressed(Key);
    DLLEXPORT static bool IsKeyPressing(Key);
    DLLEXPORT static bool IsKeyReleased(Key);
    DLLEXPORT static bool IsKeyReleasing(Key);

    DLLEXPORT static bool IsButtonPressed(MouseButton);
    DLLEXPORT static bool IsButtonPressing(MouseButton);
    DLLEXPORT static bool IsButtonReleased(MouseButton);
    DLLEXPORT static bool IsButtonReleasing(MouseButton);

    DLLEXPORT static Vec2 MousePoition();
    DLLEXPORT static const Vec2& MouseRealPoition();
    DLLEXPORT static const Vec2& MouseRelative();

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
