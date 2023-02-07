#pragma once

#include "app/handle.hpp"

class Image;
using ImageHandle = Handle<Image>;

template <typename T>
struct DuplicateEvent {
    std::vector<T> event;
};