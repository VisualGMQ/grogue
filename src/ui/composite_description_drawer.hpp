#pragma once

#include "engine/engine.hpp"
#include "ui/grid_panel.hpp"
#include "others/compose_reader.hpp"
#include "others/data.hpp"
#include "others/object_reader.hpp"

// FIXME maybe we can replace it by backpack_object_drawer?
class CompositeDescriptionDrawer final {
public:
    void operator()(component::GridPanel*, const engine::Rect& rect, int x, int y);
};
