#pragma once

#include "controller.hpp"
#include "components/sprite.hpp"
#include "components/human.hpp"
#include "components/backpack.hpp"
#include "map/map.hpp"

class BackpackController: public Controller {
public:
    BackpackController() = default;

    void Update() override;
};