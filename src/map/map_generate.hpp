#pragma once

#include "map.hpp"
#include "others/config.hpp"
#include "maptile.hpp"
#include "others/object_reader.hpp"

class MapGenerator {
public:
    virtual ~MapGenerator() = default;

    virtual std::unique_ptr<Map> Generate(int w, int h) = 0;
};

class GroundGenerator: public MapGenerator {
public:
    std::unique_ptr<Map> Generate(int w, int h) override;

private:
    enum TerrianType {
        Void = 0,
        Grass,
        Water,
        // Swamp,

        TerrianNum,
    };

    enum ObjectType {
        None = -1,
        Tree = 0,
        TreeBranch,
        BigStone,
        RoundStone,
        SharpStone,

        ObjectNum,
    };

    engine::Size size_;

    std::unique_ptr<Map> logicalTypes2Map(const std::vector<TerrianType>& terrians, const std::vector<ObjectType>& objects);

    template <typename T>
    const T& getElem(int x, int y, const std::vector<T>& m) const {
        return m[x + y * size_.w];
    }

    template <typename T>
    T& getElem(int x, int y, std::vector<T>& m) {
        return m[x + y * size_.w];
    }

    Terrian createTerrian(TerrianType);
    ObjectConfig createObject(int x, int y, Map* map, ObjectType);
};