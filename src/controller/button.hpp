#pragma once
#include "controller.hpp"

namespace controller {

class Button {
public:
    virtual ~Button() = default;

    virtual void Update() = 0;
};

}