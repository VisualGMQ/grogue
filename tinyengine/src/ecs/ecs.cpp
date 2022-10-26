#include "engine/ecs/ecs.hpp"

#include "engine/ecs/world.hpp"

namespace engine {

ECS::RemoveComponentFunc ECS::removeComponentFunc_ = nullptr;

void ECS::Init() {
    removeComponentFunc_ = std::bind(&World::RemoveComponent, World::Instance(), std::placeholders::_1);
}

void ECS::Quit() {

}

}