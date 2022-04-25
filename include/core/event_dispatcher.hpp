#pragma once

#include "pch.hpp"

namespace grogue::core {

template <typename Cont>
class EventDispatcher final {
public:
    EventDispatcher(Cont& container): container_(container) {}

    template <typename EventStructT>
    void Dispatch(const EventStructT& e) {
        for (auto& elem : container_) {
            if (e.type == SDL_KEYDOWN) {
                if (elem->OnEventKeyDown(e.key)) break;
            }
            if (e.type == SDL_KEYUP) {
                if (elem->OnEventKeyUp(e.key)) break;
            }
            if (e.type == SDL_MOUSEMOTION) {
                if (elem->OnEventMouseMotion(e.motion)) break;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (elem->OnEventMouseButtonDown(e.button)) break;
            }
            if (e.type == SDL_MOUSEBUTTONUP) {
                if (elem->OnEventMouseButtonUp(e.button)) break;
            }
        }
    }

private:
    Cont& container_;
};

}
