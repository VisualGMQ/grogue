#pragma once

#include "engine/engine.hpp"

namespace controller {

class Controller {
public:
    virtual void Update() = 0;
    virtual engine::Entity* GetEntity() { return entity_; };
    virtual void SetEntity(engine::Entity* entity) { entity_ = entity; }

private:
    engine::Entity* entity_;
};

}