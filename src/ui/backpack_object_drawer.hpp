#pragma once

#include "engine/engine.hpp"
#include "grid_object_drawer.hpp"

class BackpackObjectDrawer: public GridObjectDrawer {
public:
    DrawResources GetObject(component::GridPanel*, int index) override;
};