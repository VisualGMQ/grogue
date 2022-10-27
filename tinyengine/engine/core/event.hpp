#pragma once

#include "engine/core/pch.hpp"

namespace engine {

class EventAcceptable {
public:
    virtual ~EventAcceptable() = default;

    virtual bool OnMouseButtonDown(const SDL_MouseButtonEvent&) { return true; }
    virtual bool OnMouseButtonUp(const SDL_MouseButtonEvent&) { return true; }
    virtual bool OnMouseMotion(const SDL_MouseMotionEvent&) { return true; }
    virtual bool OnKeyDown(const SDL_KeyboardEvent&) { return true; }
    virtual bool OnKeyUp(const SDL_KeyboardEvent&) { return true; }
};

class EventDispatcher final {
public:
    void AcceptEvent(const SDL_Event&);
    void Dispatch(EventAcceptable*);
    void EventedTriggedOnUI() { isEventOnUI_ = true; }
    bool IsEventedTriggedOnUI() { return isEventOnUI_; }

    void Reset();

private:
    struct EventInfo {
        SDL_Event event;
        bool shouldDispatch;
    };
    std::vector<EventInfo> events_;
    bool isEventOnUI_;
};

class Event final {
public:
    static void Init() {}
    static void Quit() {}
    static void HandleEvent();
    static EventDispatcher& GetDispatcher() { return dispatcher_; }
    static void ClearState();

private:
    static EventDispatcher dispatcher_;
};

}