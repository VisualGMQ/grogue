#pragma once

#include <functional>

namespace engine {

class Component;

class ECS final {
public:
    static void Init();
    static void Quit();
};

}
