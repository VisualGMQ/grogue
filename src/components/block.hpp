#pragma once

#include "engine/engine.hpp"

namespace component {

class Block: public engine::Component {
public:
    Block(engine::ComponentID id): engine::Component(id) { Reset(); }
};

}