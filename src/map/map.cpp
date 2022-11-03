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

bool Map::IsInMap(int x, int y) {
    return IsInSection(x, y, 0, size_.w - 1, size_.h - 1, 0);
}

void Map::UpdateArchImage(int x, int y) {
    if (!IsInMap(x, y)) return;

    auto id = GetArchObjectID(x, y);
    if (id.has_value()) {
        auto obj = Get(x, y).object;

        engine::Vec2 top(x, y - 1);
        engine::Vec2 bottom(x, y + 1);
        engine::Vec2 left(x - 1, y);
        engine::Vec2 right(x + 1, y);

        bool adjacentUp = false;
        bool adjacentDown = false;
        bool adjacentLeft = false;
        bool adjacentRight = false;

        auto nearbyObjId = GetArchObjectID(top.x, top.y);
        if (IsInMap(top.x, top.y) && nearbyObjId.has_value() && nearbyObjId.value() == id) {
            adjacentUp = true;
        }
        nearbyObjId = GetArchObjectID(bottom.x, bottom.y);
        if (IsInMap(bottom.x, bottom.y) && nearbyObjId.has_value() && nearbyObjId.value() == id) {
            adjacentDown = true;
        }
        nearbyObjId = GetArchObjectID(left.x, left.y);
        if (IsInMap(top.x, top.y) && nearbyObjId.has_value() && nearbyObjId.value() == id) {
            adjacentLeft = true;
        }
        nearbyObjId = GetArchObjectID(right.x, right.y);
        if (IsInMap(top.x, top.y) && nearbyObjId.has_value() && nearbyObjId.value() == id) {
            adjacentRight = true;
        }

        auto sprite = obj->GetComponent<component::Sprite>();
        auto objConfig = ObjectConfigStorage::Find(id.value());

        if (adjacentLeft) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::Left];
        }
        if (adjacentRight) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::Right];
        }
        if (adjacentUp) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::Up];
        }
        if (adjacentDown) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::Down];
        }
        if (adjacentUp && adjacentLeft) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::UpLeft];
        }
        if (adjacentUp && adjacentRight) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::UpRight];
        }
        if (adjacentDown && adjacentLeft) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::DownLeft];
        }
        if (adjacentDown && adjacentRight) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::DownRight];
        }
        if (adjacentLeft && adjacentUp && adjacentDown && adjacentRight) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::Open];
        }
        if (!(adjacentLeft && adjacentUp && adjacentDown && adjacentRight)) {
            sprite->image = objConfig->orientatedArchImages[ObjectConfig::OrientationIndex::Close];
        }
    }
}

std::optional<ObjectID> Map::GetArchObjectID(int x, int y) {
    auto& tile = Get(x, y);
    if (tile.object) {
        auto arch = tile.object->GetComponent<component::Architecture>();
        if (!arch) return std::nullopt;

        return arch->id;
    }
    return std::nullopt;
}