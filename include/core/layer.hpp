#pragma once
#include "pch.hpp"
#include "animation.hpp"

namespace grogue::core {

class Scence;

class Layer {
public:
    friend class Scence;

    Layer(std::string_view name);
    virtual ~Layer() = default;

    virtual void OnInit() {}
    virtual void Update() {}
    virtual void Render() {}
    virtual void OnQuit() {}

    virtual bool OnKeyDown(const SDL_KeyboardEvent&) { return true; }
    virtual bool OnKeyUp(const SDL_KeyboardEvent&) { return true; }
    virtual bool OnMouseMotion(const SDL_MouseMotionEvent&) { return true; }
    virtual bool OnMouseButtonDown(const SDL_MouseButtonEvent&) { return true; }
    virtual bool OnMouseButtonUp(const SDL_MouseButtonEvent&) { return true; }
    virtual bool OnDropFile(const SDL_DropEvent&) { return true; }

    std::string_view GetName() const { return name_; }

    bool IsWorking() const { return isWorking_; }
    void StartWorking() { isWorking_ = true; }
    void StopWorking() { isWorking_ = false; }

    AnimationMgr& GetAnimMgr();

private:
    bool isWorking_ = true;
    std::string_view name_;
    Scence* scence_;
};

}
