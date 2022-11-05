#pragma once
#include "engine/engine.hpp"
#include "others/data.hpp"
#include "others/camera.hpp"
#include "others/config.hpp"

class CameraUpdateSystem: public engine::PerFrameSystem {
public:
    CameraUpdateSystem(engine::World* world): engine::PerFrameSystem(world) {}

    void Update() override;
};
