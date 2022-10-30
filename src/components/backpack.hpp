#pragma once

#include "engine/engine.hpp"
#include "pickupable.hpp"

namespace component {

class Backpack final: public engine::Component {
public:
    Backpack(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override {
        engine::Component::Reset();
        objects.clear();
        capacity = 20;
    }

    bool AddObject(engine::Entity* entity) {
        if (!entity) return false;

        if (objects.size() >= capacity) return false;

        auto pickupable1 = entity->GetComponent<component::Pickupable>();

        for (auto& obj : objects) {
            auto pickupable2 = obj->GetComponent<component::Pickupable>();
            if (pickupable2->id == pickupable1->id) {
                pickupable2->num += pickupable1->num;
                engine::World::Instance()->DestroyEntity(entity);
                return true;
            }
        }
        objects.push_back(entity);
        return true;
    }

    void RemoveObject(ObjectID id) {
        auto it = std::remove_if(objects.begin(), objects.end(), [&](engine::Entity* entity){
            return entity->GetComponent<component::Pickupable>()->id == id;
        });
        objects.erase(it, objects.end());
    }

    std::vector<engine::Entity*> objects;
    int capacity;
};

}
