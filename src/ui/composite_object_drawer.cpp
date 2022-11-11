#include "composite_object_drawer.hpp"

engine::Result<GridObjectDrawer::DrawResources> CompositeObjectDrawer::GetObject(component::GridPanel* panel, int index) {
    if (index >= panel->capacity || index >= ComposeConfigStorage::Size()) {
        return engine::Err{};
    }
    auto it = ComposeConfigStorage::begin();
    std::advance(it, index);
    auto config = ObjectConfigStorage::Find(it->first);
    if (!config) {
        Loge("composite object {} not found", it->first);
        return engine::Err{};
    }

    return engine::Result<GridObjectDrawer::DrawResources>(GridObjectDrawer::DrawResources{config->image, std::nullopt});
}
