#pragma once

#include "engine/engine.hpp"

namespace component {

class BoxCollider: public engine::Component {
public:
    BoxCollider(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() {
        engine::Component::Reset();

        rect.position.Set(0, 0);
        rect.size.Set(0, 0);
    }

    engine::Rect rect;
};

}