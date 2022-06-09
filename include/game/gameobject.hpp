#pragma once

#include "core/pch.hpp"
#include "core/log.hpp"
#include "component.hpp"

namespace grogue {

class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void Init() {}
    virtual void Destroy();

    template <typename T>
    void AddComponent(const std::shared_ptr<T>& component) {
        component->Init();
        auto id = ComponentIDRecorder::GetID<T>();
        if (components_.find(id) != components_.end()) {
            components_[id] = std::list<std::shared_ptr<Component>>(component);
        } else {
            components_[id].push_back(component);
        }
    }

    template <typename T>
    void RemoveComponent(const std::shared_ptr<T>& component) {
        auto id = ComponentIDRecorder::GetID<T>();
        auto it = components_.find(id);
        if (it != components_.end()) {
            it->remove(component);
        }
    }

    template <typename T>
    std::shared_ptr<Component> GetComponent() {
        auto it = components_.find(ComponentIDRecorder::GetID<T>());
        if (it == components_.end()) {
            return nullptr;
        } else {
            return *(it->begin());
        }
    }

    template <typename T>
    std::list<std::shared_ptr<Component>> GetComponents() {
        auto it = components_.find(ComponentIDRecorder::GetID<T>());
        if (it == components_.end()) {
            return std::list<std::shared_ptr<Component>>();
        } else {
            return *(it);
        }
    }

private:
    std::unordered_map<ComponentID, std::list<std::shared_ptr<Component>>> components_;
};

}
