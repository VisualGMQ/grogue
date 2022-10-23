#pragma once
#include "engine/engine.hpp"
#include "others/config.hpp"
#include "components/sprite.hpp"
#include "components/pickupable.hpp"
#include "others/data.hpp"
#include "map/map.hpp"

class HintArrowSystem: public engine::PerFrameSystem {
public:
    HintArrowSystem(engine::World* world): engine::PerFrameSystem(world) {
        engine::ImageFactory::Find("tilesheet#hint_arrow", image_);
    }

    void Update() override;

private:
    engine::Image image_;
};