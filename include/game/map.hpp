#pragma once

#include "core/pch.hpp"
#include "game/types.hpp"
#include "game/config.hpp"
#include "app/image.hpp"
#include "app/sprite.hpp"
#include "app/tilesheet.hpp"
#include "app/renderer.hpp"
#include "app/physic.hpp"

struct [[refl, luabind(comp)]] MapTile final {
    Terrian terrian;
    std::vector<Item> items;
};

struct [[refl, luabind]] Map final {
    Map(int w, int h): tiles(w, h) {}

    math::HeapMatrix<ecs::Entity> tiles;
    std::vector<ecs::Entity> items;    //!< seperate items that not on grid
    std::vector<ecs::Entity> monsters;   //!< moveable entities
};

struct [[refl, luabind(res)]] MapManager final {
public:
    void Add(std::shared_ptr<Map> map) { maps_.push_back(map); }
    const std::shared_ptr<Map>& GetCurrentMap() const { return maps_[currentIdx_]; }

private:
    std::vector<std::shared_ptr<Map>> maps_;
    int currentIdx_ = 0;
};

constexpr int MapTileSize = 16;
constexpr int MapTileRealSize = MapTileSize * SCALE;

std::shared_ptr<Map> GenDebugDemo(ecs::Commands& cmds, ecs::Resources resources, int w, int h);

void DrawMapSystem(ecs::Commands& cmd, ecs::Querier queryer,
                   ecs::Resources resources, ecs::Events& events);