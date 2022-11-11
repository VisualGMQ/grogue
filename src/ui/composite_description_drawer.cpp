#include "composite_description_drawer.hpp"

engine::Result<GridObjectDrawer::DrawResources> CompositeDescriptionDrawer::GetObject(component::GridPanel* panel, int index) {
    auto objectIdx = GameData::Instance().GetCompositePanel().Except("composite panel don't exists")
                                        ->GetComponent<component::GridPanel>().Except("composite panel don't has GridPanel component")->GetHoverIndex();
    auto it = ComposeConfigStorage::begin();
    std::advance(it, objectIdx);
    auto& materials = it->second.materials;

    int materialIdx = index;
    auto& material = materials[materialIdx];

    auto materialConfig = ObjectConfigStorage::Find(material.id);

    return engine::Ok<DrawResources>(DrawResources{materialConfig->image, material.num});
}