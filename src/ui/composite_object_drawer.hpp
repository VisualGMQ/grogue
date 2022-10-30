#pragma once

#include "engine/engine.hpp"
#include "ui/grid_panel.hpp"
#include "others/data.hpp"
#include "others/compose_reader.hpp"
#include "others/object_reader.hpp"
#include <algorithm>

class CompositeObjectDrawer final {
public:
    void operator()(component::GridPanel*, const engine::Rect& rect, int x, int y);
};
