#include "engine/core/event.hpp"
#include "engine/ui/ui.hpp"
#include "engine/core/video.hpp"
#include "engine/input/input.hpp"
#include "engine/core/timer.hpp"

namespace engine {

void EventDispatcher::AcceptEvent(const SDL_Event& event) {
    events_.push_back(EventInfo{event, true});
}

void EventDispatcher::Dispatch(EventAcceptable* accepter) {
    if (!accepter) return;
    for (auto& event : events_) {
        if (event.shouldDispatch) {
            switch (event.event.type) {
                case SDL_KEYDOWN:
                    event.shouldDispatch = accepter->OnKeyDown(event.event.key);
                    break;
                case SDL_KEYUP:
                    event.shouldDispatch = accepter->OnKeyUp(event.event.key);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    event.shouldDispatch = accepter->OnMouseButtonDown(event.event.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    event.shouldDispatch = accepter->OnMouseButtonUp(event.event.button);
                    break;
                case SDL_MOUSEMOTION:
                    event.shouldDispatch = accepter->OnMouseMotion(event.event.motion);
                    break;
            }
        }
    }
}

void EventDispatcher::Reset() {
    events_.clear();
    isEventOnUI_ = false;
}

EventDispatcher Event::dispatcher_;

void Event::HandleEvent() {
    SDL_Event event;
    UI::InputBegin();
    while (SDL_PollEvent(&event)) {
        UI::HandleEvent(&event);
        dispatcher_.AcceptEvent(event);
        if (event.type == SDL_QUIT) {
            Video::Close();
        }
        if (event.type == SDL_KEYDOWN) {
            Input::UpdateKeyState(event.key.keysym.scancode, true);
        }
        if (event.type == SDL_KEYUP) {
            Input::UpdateKeyState(event.key.keysym.scancode, false);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            Input::UpdateMouseBtnState(event.button.button, true);
        }
        if (event.type == SDL_MOUSEBUTTONUP) {
            Input::UpdateMouseBtnState(event.button.button, false);
        }
        if (event.type == SDL_MOUSEMOTION) {
            Input::UpdateMousePosition(Vec2(event.motion.x, event.motion.y),
                                       Vec2(event.motion.xrel, event.motion.yrel));
        }
        if (event.type == SDL_USEREVENT) {
            Timer::Param* param =  (Timer::Param*)event.user.data1;
            auto callback = param->owner->GetCallback();
            if (callback) {
                callback(*param->owner, param->owner->Interval(), param->userParam);
            }
        }
    }
    UI::InputEnd();
}

void Event::ClearState() {
    dispatcher_.Reset();
}

}