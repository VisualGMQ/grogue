#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/map_component.hpp"
#include "components/pickupable.hpp"
#include "components/sprite.hpp"
#include "components/architecture.hpp"
#include "others/data.hpp"

class MapRenderSystem: public engine::EntityUpdateSystem {
public:
    MapRenderSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*) override;

private:
    void drawOneTile(const engine::Image& image, const engine::Transform& transform);
    void drawOneSprite(const component::Sprite* sprite, const engine::Transform& transform);

    void drawTileAndPickupableObj(const VisiableTileArea& area, Map* map);
    void drawMonsterAndArch(const VisiableTileArea& area, Map* map);

    using Node2DPtr = engine::Node2DComponent*;
    using SpritePtr = component::Sprite*;
    void getNextMonsterInfo(MonsterManager::iterator& it, Node2DPtr& node, SpritePtr& sprite);
};