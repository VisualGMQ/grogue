#pragma once

#include "map.hpp"
#include "config.hpp"
#include "components/terrian.hpp"

class MapGenerator {
public:
    virtual ~MapGenerator() = default;

    virtual std::shared_ptr<Map> Generate(int w, int h) = 0;
};

class GroundGenerator: public MapGenerator {
public:
    std::shared_ptr<Map> Generate(int w, int h) override;

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

    std::shared_ptr<Map> logicalTypes2Map(const std::vector<TerrianType>& terrians, const std::vector<ObjectType>& objects);

    template <typename T>
    const T& getElem(int x, int y, const std::vector<T>& m) const {
        return m[x + y * size_.w];
    }

    template <typename T>
    T& getElem(int x, int y, std::vector<T>& m) {
        return m[x + y * size_.w];
    }

    engine::Entity* createTerrian(int x, int y, TerrianType);
    engine::Entity* createObject(int x, int y, ObjectType);
};

class DungeonGenerator: public MapGenerator {
public:
    std::shared_ptr<Map> Generate(int w, int h) override;
};