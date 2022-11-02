#include "composite_object_drawer.hpp"

GridObjectDrawer::DrawResources CompositeObjectDrawer::GetObject(component::GridPanel* panel, int index) {
    if (index >= panel->capacity || index >= ComposeConfigStorage::Size()) {
        return {};
    }
    auto it = ComposeConfigStorage::begin();
    std::advance(it, index);
    auto config = ObjectConfigStorage::Find(it->first);
    if (!config) {
        Loge("composite object {} not found", it->first);
        return {};
    }

    return {config->image, std::nullopt};
}
