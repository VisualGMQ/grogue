#include "feature.hpp"

namespace component {

void Feature::Reset() {
    engine::Component::Reset();

    feature.cut = 0;
    feature.beat = 0;

    feature.beBeated = 0;
    feature.beCutted = 0;

    feature.dropObjects.clear();
}

}