#pragma once
#include "engine/engine.hpp"
#include "map/map.hpp"

namespace component {

class MapComponent: public engine::Component {
public:
    MapComponent(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override {
        engine::Component::Reset();
        map = nullptr;
    }

    Map* map; 
};

} // namespace component
