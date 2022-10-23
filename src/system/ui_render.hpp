#pragma once

#include "engine/engine.hpp"
#include "ui/backpack_panel.hpp"
#include "others/data.hpp"
#include "components/backpack.hpp"

class UIRenderSystem: public engine::EntityUpdateSystem {
public:
    UIRenderSystem(engine::World* world): engine::EntityUpdateSystem(world) {}

    void Update(engine::Entity*);
};