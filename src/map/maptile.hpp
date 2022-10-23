#pragma once
#include "engine/engine.hpp"
#include "others/object_config.hpp"

struct Terrian {
    engine::Image image;

    enum Type {
        Liquid,
        Solid,
        Void
    }type = Void;
};

class MapTile {
public:
    Terrian terrian;
    engine::Entity* object = nullptr;
    // bool canSee = false;
    // bool visited = false;
};