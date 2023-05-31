#pragma once

#include "core/pch.hpp"

template <typename T>
struct DuplicateEvent {
    std::vector<T> events;

    operator bool() const {
        return !events.empty();
    }
};

using KeyboardEvents = DuplicateEvent<SDL_KeyboardEvent>;
using MouseBtnEvents = DuplicateEvent<SDL_MouseButtonEvent>;
using QuitEvent = SDL_QuitEvent;
using MouseMotionEvent = SDL_MouseMotionEvent;
using MouseWheelEvent = SDL_MouseWheelEvent;