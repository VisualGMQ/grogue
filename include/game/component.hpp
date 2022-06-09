#pragma once

#include <type_traits>

namespace grogue {

class Component {
public:
    virtual ~Component() = default;

    virtual void Init() {}
    virtual void Destroy() {}
    virtual void Update() {}

    void Active() { isActive_ = true; }
    void InActive() { isActive_ = false; }
    bool IsActive() const { return isActive_; }

private:
    bool isActive_ = false;
};

using ComponentID = unsigned int;

class ComponentIDRecorder final {
public:
    template <typename T>
    static unsigned int GetID() {
        static_assert(std::is_base_of_v<Component, T> &&
                      !std::is_same_v<Component, T>);
        static ComponentID id = ++ index_;
        return id;
    }

private:
    static ComponentID index_;
};

}
