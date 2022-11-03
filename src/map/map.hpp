#pragma once

#include "engine/engine.hpp"
#include "maptile.hpp"
#include "others/config.hpp"
#include "others/common.hpp"
#include "components/architecture.hpp"
#include "components/sprite.hpp"
#include "others/object_reader.hpp"

class Map {
public:
    Map(int w, int h): size_(w, h) {
        tiles_.resize(w * h);
    }
    MapTile& Get(int x, int y) { return tiles_[x * size_.w + y]; }
    const engine::Size& GetSize() const { return size_; }
    void UpdateArchImage(int x, int y);
    std::optional<ObjectID> GetArchObjectID(int x, int y);
    bool IsInMap(int x, int y);

private:
    std::vector<MapTile> tiles_;
    engine::Size size_;
};

class MapManager final {
public:
    static void SetGroundMap(std::unique_ptr<Map>&& m) { groundMap_ = std::move(m); m = nullptr; }
    static Map* GetGroundMap() { return groundMap_.get(); }
    static bool HasDungeon() { return !dungeonMap_.empty(); }
    static Map* GetCurDungeonMap() { return dungeonMap_[curDungeonMapIdx_].get(); }
    static void ClearGroundMap() { groundMap_.reset(); }
    static void ClearDungeonMap() { dungeonMap_.clear(); }

private:
    static std::unique_ptr<Map> groundMap_;
    static std::vector<std::unique_ptr<Map>> dungeonMap_;
    static int curDungeonMapIdx_;
};

struct VisiableTileArea {
    int initCols;
    int initRows;
    int endCols;
    int endRows;
    engine::Vec2 offset;
};

VisiableTileArea CalcVisiableTileArea(Map* map, const engine::Vec2& position);