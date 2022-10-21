#pragma once
#include "engine/engine.hpp"
#include "object_config.hpp"

struct Terrian {
    engine::Image image;

    enum Type {
        Liquid,
        Solid,
        Void
    }type;
};

class MapTile {
public:
    Terrian terrian;
    std::optional<ObjectConfig> object;
    bool canSee = false;
    bool visited = false;
};