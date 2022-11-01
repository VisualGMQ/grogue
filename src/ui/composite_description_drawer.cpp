#include "composite_description_drawer.hpp"

GridObjectDrawer::DrawResources CompositeDescriptionDrawer::GetObject(component::GridPanel* panel, int index) {
    auto compositePanel = GameData::Instance()->GetCompositePanel()->GetComponent<component::GridPanel>();
    auto& hoverPos = compositePanel->GetHoverGridPos();
    int objectIdx = hoverPos.x + hoverPos.y * panel->gridNumInCol;
    auto it = ComposeConfigStorage::begin();
    std::advance(it, objectIdx);
    auto& materials = it->second.materials;

    int materialIdx = panel->GetHoverGridPos().x + panel->GetHoverGridPos().y * panel->gridNumInCol;
    auto& material = materials[materialIdx];

    auto materialConfig = ObjectConfigStorage::Find(material.id);

    return {materialConfig->image, std::nullopt};
}