#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "others/data.hpp"
#include "collision/collision.hpp"
#include "components/box_collider.hpp"
#include "components/rigidbody.hpp"
#include "components/architecture.hpp"
#include "map/map.hpp"

class CollisionSystem: public engine::PerFrameSystem {
public:
    CollisionSystem(engine::World* world): engine::PerFrameSystem(world) {}

    void Update() override;

private:
    void moveMonsterIntoMap(Map* map, engine::Node2DComponent* node2d, const engine::Rect& globalColliBox, const engine::Rect& localColliBox, const engine::Size& imgSize);
    void collideMonsterWithMapTile(const engine::Rect& archRect, engine::Node2DComponent* monsterNode, component::RigidBody* monsterRigid, engine::Rect& box);

    void collideMonsterAndMap(Map*);
};