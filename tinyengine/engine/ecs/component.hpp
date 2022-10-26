#pragma once

#include "engine/core/pch.hpp"

namespace engine {

class Entity;

using ComponentID = unsigned int;

class Component {
public:
    friend class Entity;
    friend class World;

    Component(ComponentID id) : id_(id), parent_(nullptr), isActive_(true) { }
    virtual ~Component() = default;

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    virtual void Reset() {
        parent_ = nullptr;
        isActive_ = true;
    }
    Entity* Parent() const { return parent_; }
    bool IsActive() const { return isActive_; }
    void Active(bool active) { isActive_ = active; }

    unsigned int ID() const { return id_; }

private:
    ComponentID id_;
    bool isActive_;
    Entity* parent_ = nullptr;
};

class ComponentIDHelper final {
public:
    template <typename T>
    static unsigned int GetID() {
        static_assert(std::is_base_of_v<Component, T> && !std::is_same_v<Component, T>);
        static unsigned int id = curIdx_++;
        return id;
    }

private:
    static unsigned int curIdx_;
};


}
