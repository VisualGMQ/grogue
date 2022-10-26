#pragma once

#include <functional>

namespace engine {

class Component;

class ECS final {
public:
    using RemoveComponentFunc = std::function<void(Component*)>;

    static void Init();
    static void Quit();
    static RemoveComponentFunc GetRemoveFunc() { return removeComponentFunc_; }

private:
    static RemoveComponentFunc removeComponentFunc_;
};

}