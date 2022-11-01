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
};