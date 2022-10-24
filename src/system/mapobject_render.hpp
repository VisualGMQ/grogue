#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/map_component.hpp"
#include "components/architecture.hpp"
#include "components/sprite.hpp"
#include "others/data.hpp"

class MapObjectRenderSystem: public engine::EntityUpdateSystem {
public:
    MapObjectRenderSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*) override;

private:
    MonsterManager::iterator monsterIt_;

    void drawMonster(int y);
    void drawTile(MapTile& tile, engine::Node2DComponent* node,  const VisiableTileArea& visiableArea, int x, int y);
};