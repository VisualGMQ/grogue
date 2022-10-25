#pragma once

#include "engine/ecs/system.hpp"
#include "engine/renderer/renderer.hpp"
#include "engine/ecs/world.hpp"
#include "engine/ecs/entity.hpp"
#include "engine/input/input.hpp"

namespace engine {

class DLLEXPORT UISystem final: public EntityUpdateSystem {
public:
    UISystem(class World* world): EntityUpdateSystem(world) {}

    void Update(Entity*) override;
};

}
