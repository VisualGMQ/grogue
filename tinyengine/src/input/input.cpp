#include "engine/input/input.hpp"

namespace engine {

bool Input::oldKeyState_[SDL_NUM_SCANCODES] = {0};
bool Input::keyState_[SDL_NUM_SCANCODES] = {0};
bool Input::oldBtnState_[MOUSE_BTN_NUM] = {0};
bool Input::btnState_[MOUSE_BTN_NUM] = {0};
Vec2 Input::position_;
Vec2 Input::offset_;

void Input::UpdateKeyState(Key key, bool isPress) {
    keyState_[key] = isPress;
}

void Input::UpdateMouseBtnState(Uint8 btn, bool isPress) {
    btnState_[btn] = isPress;
}

void Input::UpdateMousePosition(const Vec2& position, const Vec2& relpos) {
    offset_ = relpos;
    position_ = position;
}

void Input::Init() {
    memset(oldKeyState_, 0, SDL_NUM_SCANCODES);
    memset(keyState_, 0, SDL_NUM_SCANCODES);
    memset(oldBtnState_, 0, MOUSE_BTN_NUM);
    memset(btnState_, 0, MOUSE_BTN_NUM);
    position_.Set(0, 0);
    offset_.Set(0, 0);
}

void Input::Quit() { }


bool Input::IsKeyPressed(Key code) {
    return !oldKeyState_[code] && keyState_[code];
}

bool Input::IsKeyPressing(Key code) {
    return oldKeyState_[code] && keyState_[code] || IsKeyPressed(code);
}

bool Input::IsKeyReleased(Key code) {
    return oldKeyState_[code] && !keyState_[code];
}

bool Input::IsKeyReleasing(Key code) {
    return !oldKeyState_[code] && !keyState_[code] || IsKeyReleased(code);
}

bool Input::IsButtonPressed(MouseButton button) {
    return !oldBtnState_[button] && btnState_[button];
}

bool Input::IsButtonPressing(MouseButton button) {
    return oldBtnState_[button] && btnState_[button] || IsButtonPressed(button);
}

bool Input::IsButtonReleased(MouseButton button) {
    return oldBtnState_[button] && !btnState_[button];
}

bool Input::IsButtonReleasing(MouseButton button) {
    return !oldBtnState_[button] && !btnState_[button] || IsButtonReleasing(button);
}

Vec2 Input::MousePoition() {
    auto size = Video::GetWindowSize();
    auto& initSize = Video::GetInitSize();
    return position_ * Vec2(initSize.w / size.w, initSize.h / size.h);
}

const Vec2& Input::MouseRelative() {
    return offset_;
}

const Vec2& Input::MouseRealPoition() {
    return position_;
}

void Input::UpdateStates() {
    memcpy(oldBtnState_, btnState_, sizeof(oldBtnState_));
    memcpy(oldKeyState_, keyState_, sizeof(oldKeyState_));
    offset_.Set(0, 0);
}

}
