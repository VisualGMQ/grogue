#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/map_component.hpp"
#include "components/sprite.hpp"

class MapObjectRenderSystem: public engine::EntityUpdateSystem {
public:
    MapObjectRenderSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*) override;
};