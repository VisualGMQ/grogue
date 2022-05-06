#pragma once
#include "pch.hpp"

namespace grogue::core {

class Layer {
public:
    Layer(std::string_view name);
    virtual ~Layer() = default;

    virtual void OnInit() {}
    virtual void Update() {}
    virtual void Render() {}
    virtual void OnQuit() {}

    virtual bool OnEventKeyDown(const SDL_KeyboardEvent&) { return true; }
    virtual bool OnEventKeyUp(const SDL_KeyboardEvent&) { return true; }
    virtual bool OnEventMouseMotion(const SDL_MouseMotionEvent&) { return true; }
    virtual bool OnEventMouseButtonDown(const SDL_MouseButtonEvent&) { return true; }
    virtual bool OnEventMouseButtonUp(const SDL_MouseButtonEvent&) { return true; }
    virtual bool OnEventDropFile(const SDL_DropEvent&) { return true; }

    std::string_view GetName() const { return name_; }

    bool IsWorking() const { return isWorking_; }
    void StartWorking() { isWorking_ = true; }
    void StopWorking() { isWorking_ = false; }

private:
    bool isWorking_ = true;
    std::string_view name_;
};

}
