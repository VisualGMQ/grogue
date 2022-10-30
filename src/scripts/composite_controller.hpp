#pragma once

#include "engine/engine.hpp"
#include "controller.hpp"
#include "ui/grid_panel.hpp"
#include "others/compose_reader.hpp"
#include <algorithm>

class CompositeController: public Controller {
public:
    void Update() override;

private:
    void changeDescriptionPanelCapacity(component::GridPanel* panel);
};
