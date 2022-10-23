#include "map.hpp"

std::unique_ptr<Map> MapManager::groundMap_;
std::vector<std::unique_ptr<Map>> MapManager::dungeonMap_;
int MapManager::curDungeonMapIdx_ = 0;

VisiableTileArea CalcVisiableTileArea(Map* map, const engine::Vec2& position) {
    if (!map) { return VisiableTileArea{0, 0, 0, 0}; }

    auto size = map->GetSize();

    VisiableTileArea area;

    area.initCols = std::floor(position.x / TileSize);
    area.initRows = std::floor(position.y / TileSize);
    area.endCols = std::min(std::ceil(engine::Video::GetInitSize().w / TileSize), size.w);
    area.endRows = std::min(std::ceil(engine::Video::GetInitSize().h / TileSize), size.h);

    area.offset = position;

    return area;
}