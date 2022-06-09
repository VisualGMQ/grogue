#pragma once

#include "assert.h"

namespace grogue::core {

template <typename T>
class Sington {
public:
    static T instance_;

    static T& GetInstance() {
        GROGUE_ASSERT(instance_, "Sington instance is nullptr");
        return *instance_;
    }
};

template <typename T>
T Sington<T>::instance_ = nullptr;

}
