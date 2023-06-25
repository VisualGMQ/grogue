#pragma once

#include "core/pch.hpp"
#include "app/input.hpp"

struct LUA_BIND InputActionState final {
    enum State {
        Unknown,
        Pressed,
        Pressing,
        Released,
        Releasing,
    };

    LUA_NOBIND InputActionState(State state): state_(state) {}

    bool IsPressed() const { return state_ == Pressed;}
    bool IsPressing() const { return state_ == Pressing; }
    bool IsReleased() const { return state_ == Released; }
    bool IsReleasing() const { return state_ == Releasing; }

private:
    State state_ = State::Unknown;
};

//! @brief an input wrapper for hidding different between keyboard and touch devices
class RawInput {
public:
    virtual InputActionState GetActionState(const std::string& action) const = 0;
    virtual math::Vector2 Axis() const = 0;

    virtual ~RawInput() = default;
};

//! @brief  input for keyboard devices
class KeyboardInput : public RawInput {
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
class TouchInput : public RawInput {
    InputActionState GetActionState(const std::string& action) const override;
    math::Vector2 Axis() const override;
};


//! @brief a wrapper for RawInput, used for bind in Lua
class LUA_BIND_RESOURCE Input final {
public:
    LUA_NOBIND Input() = default;
    LUA_NOBIND Input(std::unique_ptr<RawInput>&& input): input_(std::move(input)) {}

    InputActionState  GetActionState(const std::string& action) const {
        Assert(input_, "input is nullptr");
        return input_->GetActionState(action);
    }

    math::Vector2 Axis() const {
        Assert(input_, "input is nullptr");
        return input_->Axis();
    }

private:
    std::unique_ptr<RawInput> input_;
};