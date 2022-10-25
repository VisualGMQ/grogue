#pragma once

#include "engine/engine.hpp"
#include "others/object_config.hpp"

class Compose : public engine::Component {
public:
    Compose(engine::ComponentID id): engine::Component(id) { Reset(); }
};