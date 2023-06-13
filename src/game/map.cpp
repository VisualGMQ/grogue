#include "game/map.hpp"

std::shared_ptr<Map> GenDebugDemo(ecs::Resources resources, int w, int h) {
    auto logicMap = math::HeapMatrix<TerrianType>(w, h);
    logicMap.Fill(TerrianType::DryLand);

    std::shared_ptr<Map> map = std::make_shared<Map>(w, h);

    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    auto tilesheet = tilesheetMgr.Find("tilesheet");

    auto& gameConfig = resources.Get<GameConfig>();
    auto& itemConfig = gameConfig.GetItemConfig();

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
            for (auto& itemInfo : itemConfig.Items()) {
                if (Random::Instance().RandRange(0, itemConfig.TotleWeight()) < itemInfo.second.weight) {
                    Item item;
                    item.nameID = itemInfo.second.name;
                    items.push_back(item);
                    break;
                }
            }
            map->tiles.Set(x, y, MapTile{terrian, items});
        }
    }

    return map;
}

void DrawMapSystem(ecs::Commands& cmd, ecs::Querier queryer,
                   ecs::Resources resources, ecs::Events& events) {
    auto& mapMgr = resources.Get<MapManager>();
    auto& renderer = resources.Get<Renderer>();
    auto& gameConfig = resources.Get<GameConfig>();

    auto& map = mapMgr.GetCurrentMap();
    for (int y = 0; y < map->tiles.Height(); y++) {
        for (int x = 0; x < map->tiles.Width(); x++) {
            const auto& tile = map->tiles.Get(x, y);
            Transform transform = Transform::Create(
                math::Vector2(x * MapTileRealSize, y * MapTileRealSize),
                0, math::Vector2(SCALE, SCALE));
            renderer.DrawSprite(tile.terrian.sprite, transform);

            for (const auto& item : tile.items) {
                const auto& info = gameConfig.GetItemConfig().Items().at(item.nameID);
                renderer.DrawSprite(info.sprite, transform);
            }
        }
    }
}
