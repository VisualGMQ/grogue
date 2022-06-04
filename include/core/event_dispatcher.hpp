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
                if (elem->OnKeyDown(e.key)) break;
            }
            if (e.type == SDL_KEYUP) {
                if (elem->OnKeyUp(e.key)) break;
            }
            if (e.type == SDL_MOUSEMOTION) {
                if (elem->OnMouseMotion(e.motion)) break;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (elem->OnMouseButtonDown(e.button)) break;
            }
            if (e.type == SDL_MOUSEBUTTONUP) {
                if (elem->OnMouseButtonUp(e.button)) break;
            }
            if (e.type == SDL_DROPFILE) {
                if (elem->OnDropFile(e.drop)) break;
            }
            if (e.type == SDL_DROPTEXT) {
                if (elem->OnDropFile(e.drop)) break;
            }
        }
    }

private:
    Cont& container_;
};

}
