#pragma once

#include "engine/ecs/component.hpp"

namespace engine {

class UI final {
public:
    static void Init();
    static void Quit();
    static void Update();
    static void HandleEvent(SDL_Event*);
    static void InputBegin();
    static void InputEnd();

private:
};

}
