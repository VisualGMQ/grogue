#include "game/map.hpp"

std::shared_ptr<Map> GenDebugDemo(ecs::Commands& cmds, ecs::Resources resources, int w, int h) {
    auto logicMap = math::HeapMatrix<TerrianType>(w, h);
    logicMap.Fill(TerrianType::DryLand);

    std::shared_ptr<Map> map = std::make_shared<Map>(w, h);

    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    auto tilesheet = tilesheetMgr.Find("tilesheet");

    const auto& gameConfig = resources.Get<GameConfig>();
    const auto& itemConfig = gameConfig.GetItemConfig();

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            auto tile = tilesheet.Get(0, Random::Instance().RandRange(10, 11));
            auto sprite = Sprite::FromRegion(tile.region);
            sprite.customSize.Set(MapTileSize, MapTileSize);
            Terrian terrian = {
                logicMap.Get(x, y),
                EntityProperty{
                               1.0, 0.0,
                               0.0, 0.0,
                               },
                SpriteBundle{
                               sprite, tile.handle,
                               }
            };

            std::vector<Item> items;
            bool hasArchitecture = false;
            for (auto& itemInfo : itemConfig.Items()) {
                if (Random::Instance().RandRange(0, itemConfig.TotleWeight()) < itemInfo.second.weight) {
                    Item item;
                    item.nameID = itemInfo.second.name;
                    hasArchitecture = hasArchitecture || itemConfig.Items().find(item.nameID)->second.architecture;
                    items.push_back(item);
                    break;
                }
            }
            auto entity = cmds.SpawnAndReturn<MapTile>(MapTile{terrian, items});
            if (hasArchitecture) {
                auto tileSize = math::Vector2(MapTileRealSize, MapTileRealSize);
                cmds.AddComponent<physic::CollideShape>(
                    entity,
                    physic::CollideShape{std::make_shared<physic::AABB>(
                        tileSize * math::Vector2(x, y) + tileSize / 2.0,
                        tileSize / 2.0)});
            }
            map->tiles.Set(x, y, entity);
        }
    }

    return map;
}

void DrawMapSystem(ecs::Commands& cmd, ecs::Querier querier,
                   ecs::Resources resources, ecs::Events& events) {
    auto& mapMgr = resources.Get<MapManager>();
    auto& renderer = resources.Get<Renderer>();
    auto& gameConfig = resources.Get<GameConfig>();

    auto& map = mapMgr.GetCurrentMap();
    for (int y = 0; y < map->tiles.Height(); y++) {
        for (int x = 0; x < map->tiles.Width(); x++) {
            ecs::Entity tileEntity = map->tiles.Get(x, y);
            Transform transform = Transform::Create(
                math::Vector2(x * MapTileRealSize, y * MapTileRealSize),
                0, math::Vector2(SCALE, SCALE));
            MapTile& tile = querier.Get<MapTile>(tileEntity);
            renderer.DrawSprite(tile.terrian.sprite, transform);

            for (const auto& item : tile.items) {
                const auto& info = gameConfig.GetItemConfig().Items().at(item.nameID);
                renderer.DrawSprite(info.sprite, transform);
            }
        }
    }
}
