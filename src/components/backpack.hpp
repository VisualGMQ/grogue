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

    engine::Result<void> AddObject(engine::Entity* entity) {
        if (!entity) return engine::Err{};

        if (objects.size() >= capacity) return engine::Err{};

        component::Pickupable* pickupable1;
        PROPAGATING(entity->GetComponent<component::Pickupable>(), pickupable1);

        for (auto& obj : objects) {
            component::Pickupable* pickupable2;
            PROPAGATING(obj->GetComponent<component::Pickupable>(), pickupable2);

            if (pickupable2->id == pickupable1->id) {
                pickupable2->num += pickupable1->num;
                engine::World::Instance().DestroyEntity(entity);
                return engine::Ok<void>{};
            }
        }
        objects.push_back(entity);
        return engine::Ok<void>{};
    }

    void RemoveObject(ObjectID id) {
        auto it = std::remove_if(objects.begin(), objects.end(), [&](engine::Entity* entity){
            auto pickup = entity->GetComponent<component::Pickupable>();
            if (pickup.IsErr()) return false;
            return pickup.Unwrap()->id == id;
        });
        objects.erase(it, objects.end());
    }

    std::vector<engine::Entity*> objects;
    int capacity;
};

}
