#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/map_component.hpp"

class MapTileRenderSystem: public engine::EntityUpdateSystem {
public:
    MapTileRenderSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*) override;
};