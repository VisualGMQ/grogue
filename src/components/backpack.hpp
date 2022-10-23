#pragma once

#include "engine/engine.hpp"

namespace component {

class Backpack final: public engine::Component {
public:
    Backpack(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override { objects.clear(); }

    std::vector<ObjectConfig> objects;
};

} // namespace component
