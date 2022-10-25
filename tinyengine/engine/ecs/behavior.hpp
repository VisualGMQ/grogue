#pragma once

#include "engine/core/pch.hpp"
#include "engine/core/event.hpp"
#include "engine/ecs/system.hpp"

namespace engine {

class Entity;
class BoxColliderComponent;

class DLLEXPORT Behavior: public EventAcceptable {
public:
    friend class Entity;

    Behavior(const Behavior&) = delete;
    Behavior& operator=(const Behavior&) = delete;

    Behavior() = default;
    virtual ~Behavior() = default;

    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnQuit() = 0;
    virtual void BeforeCollide(BoxColliderComponent*) {}
    virtual void AfterCollide(BoxColliderComponent*) {}

    bool IsInited() const { return isInited_; }
    void Inited() { isInited_ = true; }
    Entity* Parent() const { return parent_; }

private:
    bool isInited_ = false;
    Entity* parent_;
};

}
