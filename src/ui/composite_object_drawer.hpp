#pragma once

#include "engine/engine.hpp"
#include "ui/grid_panel.hpp"
#include "others/data.hpp"
#include "others/compose_reader.hpp"
#include "others/object_reader.hpp"
#include "others/common.hpp"
#include "ui/grid_object_drawer.hpp"

class CompositeObjectDrawer: public GridObjectDrawer {
public:
    engine::Result<DrawResources> GetObject(component::GridPanel* panel, int index) override;
};
