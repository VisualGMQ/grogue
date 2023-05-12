#pragma once

#include "core/pch.hpp"
#include "game/types.hpp"
#include "app/image.hpp"
#include "app/sprite.hpp"

struct MapTile final {
    Terrian terrian;
    Item item;
};

struct Map final {
    math::HeapMatrix<MapTile> tiles;
    std::vector<Item> items;    //!< seperate items that not on grid
    std::vector<ecs::Entity> entities;   //!< moveable entities
};
