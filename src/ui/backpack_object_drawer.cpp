#include "backpack_object_drawer.hpp"

engine::Result<GridObjectDrawer::DrawResources> BackpackObjectDrawer::GetObject(component::GridPanel*, int index) {
    engine::Entity* player;
    PROPAGATING(GameData::Instance().GetPlayer(), player);

    component::Backpack* backpack;
    PROPAGATING(player->GetComponent<component::Backpack>(), backpack);

    if (backpack->objects.size() <= index) { return engine::Err{}; }

    auto obj = backpack->objects[index];

    component::Sprite* sprite;
    PROPAGATING(obj->GetComponent<component::Sprite>(), sprite);

    component::Pickupable* pickupable;
    PROPAGATING(obj->GetComponent<component::Pickupable>(), pickupable);

    return engine::Result<GridObjectDrawer::DrawResources>(GridObjectDrawer::DrawResources{sprite->image, pickupable->num});
}