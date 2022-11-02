#include "backpack_object_drawer.hpp"

GridObjectDrawer::DrawResources BackpackObjectDrawer::GetObject(component::GridPanel*, int index) {
    auto player = GameData::Instance()->GetPlayer();
    if (!player) return {{}, 0};

    auto backpack = player->GetComponent<component::Backpack>();
    if (!backpack) return {{}, 0};

    if (backpack->objects.size() <= index) { return {{}, 0}; }

    auto obj = backpack->objects[index];

    auto sprite = obj->GetComponent<component::Sprite>();
    auto pickupable = obj->GetComponent<component::Pickupable>();
    return {sprite->image, pickupable->num};
}