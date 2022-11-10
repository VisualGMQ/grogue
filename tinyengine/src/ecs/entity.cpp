#include "engine/ecs/entity.hpp"
#include "engine/ecs/world.hpp"

namespace engine {

void Entity::RemoveComponent(Component* component) {
    for (auto it = components_.begin(); it != components_.end(); it++) {
        if (it->second == component) {
            components_.erase(it);
            World::Instance().RemoveComponent(component);
            return;
        }
    }
}

}
