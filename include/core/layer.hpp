#pragma once
#include "pch.hpp"

namespace grogue::core {

class Layer {
public:
    Layer(std::string_view name);
    virtual ~Layer() = default;

    virtual void Update() {}
    virtual void Render() {}

    template <Uint32 EventT, typename EventStructT>
    bool Handle(const EventStructT&) { return true; }

    template <>
    virtual bool Handle<SDL_KEYDOWN>(const SDL_KeyboardEvent&) { return true; }

    template <>
    virtual bool Handle<SDL_KEYUP>(const SDL_KeyboardEvent&) { return true; }

    template <>
    virtual bool Handle<SDL_MOUSEMOTION>(const SDL_MouseMotionEvent&) { return true; }

    template <>
    virtual bool Handle<SDL_MOUSEBUTTONDOWN>(const SDL_MouseButtonEvent&) { return true; }

    template <>
    virtual bool Handle<SDL_MOUSEBUTTONUP>(const SDL_MouseButtonEvent&) { return true; }

    std::string_view GetName() const { return name_; }

    bool IsWorking() const { return isWorking_; }
    void StartWorking() { isWorking_ = true; }
    void StopWorking() { isWorking_ = false; }

private:
    bool isWorking_ = true;
    std::string_view name_;
};

}
