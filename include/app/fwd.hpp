#pragma once

#include "app/handle.hpp"
#include "SDL.h"

class Image;
using ImageHandle = Handle<Image>;

template <typename T>
struct DuplicateEvent {
    std::vector<T> events;

    operator bool() const {
        return !events.empty();
    }
};

using KeyboardEvents = DuplicateEvent<SDL_KeyboardEvent>;
using MouseBtnEvents = DuplicateEvent<SDL_MouseButtonEvent>;
