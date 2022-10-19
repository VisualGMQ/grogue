#pragma once

#include <vector>

using ObjectID = unsigned int;

struct ObjectFeature {
    int cut;
    int beat;

    int beBeated;
    int beCutted;

    struct DropObject {
        std::string name;
        int num;
    };
    std::vector<DropObject> dropObjects;
};