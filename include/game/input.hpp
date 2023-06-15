#pragma once

#include "core/pch.hpp"
#include "app/input.hpp"

struct InputActionState final {
    enum State {
        Unknown,
        Pressed,
        Pressing,
        Released,
        Releasing,
    };

    InputActionState(State state): state_(state) {}

    bool IsPressed() const { return state_ == Pressed;}
    bool IsPressing() const { return state_ == Pressing; }
    bool IsReleased() const { return state_ == Released; }
    bool IsReleasing() const { return state_ == Releasing; }

private:
    State state_;
};

//! @brief an input wrapper for hidding different between keyboard and touch devices
class Input {
public:
    virtual InputActionState GetActionState(const std::string& action) const = 0;
    virtual math::Vector2 Axis() const = 0;

    virtual ~Input() = default;
};

using InputPtr = std::unique_ptr<Input>;

//! @brief  input for keyboard devices
class KeyboardInput : public Input {
public:
    KeyboardInput(Keyboard& keyboard, std::unordered_map<std::string, Key>&& actions): keyboard_(keyboard), actions_(std::move(actions)) {}
    KeyboardInput(Keyboard& keyboard, const std::unordered_map<std::string, Key>& actions): keyboard_(keyboard), actions_(actions) {}

    InputActionState GetActionState(const std::string& action) const override;
    math::Vector2 Axis() const override;

private:
    std::unordered_map<std::string, Key> actions_;
    Keyboard& keyboard_;
};

//! @brief  input for touchable devices(like Phone)
class TouchInput : public Input {
    InputActionState GetActionState(const std::string& action) const override;
    math::Vector2 Axis() const override;
};