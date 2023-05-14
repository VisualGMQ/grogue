#include "game/map.hpp"

std::shared_ptr<Map> GenDebugDemo(ecs::Resources resources, int w, int h) {
    auto logicMap = math::HeapMatrix<Terrian::Type>(w, h);
    logicMap.Fill(Terrian::Type::DryLand);

    std::shared_ptr<Map> map = std::make_shared<Map>(w, h);

    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    auto tilesheet = tilesheetMgr.Find("tilesheet");

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
            map->tiles.Set(x, y, MapTile{terrian, {}});
        }
    }

    return map;
}

void DrawMapSystem(ecs::Commands& cmd, ecs::Querier queryer,
                   ecs::Resources resources, ecs::Events& events) {
    auto& mapMgr = resources.Get<MapManager>();
    auto& renderer = resources.Get<Renderer>();

    auto& map = mapMgr.GetCurrentMap();
    for (int y = 0; y < map->tiles.Height(); y++) {
        for (int x = 0; x < map->tiles.Width(); x++) {
            const auto& tile = map->tiles.Get(x, y);
            Transform transform = Transform::Create(
                math::Vector2(x * MapTileSize * SCALE, y * MapTileSize * SCALE),
                0, math::Vector2(SCALE, SCALE));
            renderer.DrawSprite(tile.terrian.sprite, transform);
        }
    }
}