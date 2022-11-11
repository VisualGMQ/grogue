#pragma once

#include "grid_object_drawer.hpp"
#include "others/compose_reader.hpp"
#include "others/object_reader.hpp"

class CompositeDescriptionDrawer: public GridObjectDrawer {
public:
    engine::Result<DrawResources> GetObject(component::GridPanel*, int index) override;
};
