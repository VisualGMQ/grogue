#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/pickupable.hpp"
#include "components/human.hpp"
#include "others/data.hpp"
#include "map/map.hpp"

class PutHintSystem: public engine::PerFrameSystem {
public:
    PutHintSystem(engine::World* world): engine::PerFrameSystem(world) {
    }

    void Update() override;
};