#pragma once

#include "engine/engine.hpp"
#include "map/object_feature.hpp"

namespace component {

class Feature: public engine::Component {
public:
    Feature(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override;

    ObjectFeature feature;
};

}