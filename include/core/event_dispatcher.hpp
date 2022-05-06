#pragma once

#include "pch.hpp"

namespace grogue::core {

template <typename Cont>
class EventDispatcher final {
public:
    EventDispatcher(Cont& container): container_(container) {}

    void Dispatch(const SDL_Event& e) {
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
            if (e.type == SDL_DROPFILE) {
                if (elem->OnEventDropFile(e.drop)) break;
            }
            if (e.type == SDL_DROPTEXT) {
                if (elem->OnEventDropFile(e.drop)) break;
            }
        }
    }

private:
    Cont& container_;
};

}
