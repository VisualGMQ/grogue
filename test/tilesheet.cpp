#include "app/app.hpp"
#include "test_helper.hpp"

void LoadResourcesSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& manager = resources.Get<TilesheetManager>();
    auto tilesheet = manager.LoadFromConfig(TestHelper::Instance().GetResourcePath() + "font_desc.lua");
    cmd.SetResource<Tilesheet>(std::move(tilesheet));
}

void UpdateSystem(ecs::Commands& cmd, ecs::Querier queryer, ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();

    auto& tilesheet = resources.Get<Tilesheet>();
    auto tileSize = tilesheet.TileSize();
    for (int x = 0; x < tilesheet.Col(); x++) {
        for (int y = 0; y < tilesheet.Row(); y++) {
            auto tile = tilesheet.Get(x, y);
            Transform transform;
            transform.position = math::Vector2{x * (tileSize.x + 25), y * (tileSize.y + 25)};
            Sprite sprite = Sprite::Default();
            sprite.customSize.x = tile.region.w;
            sprite.customSize.y = tile.region.h;
            sprite.region = tile.region;
            SpriteBundle bundle{sprite, tilesheet.Handle()};
            renderer.DrawSprite(bundle, transform);
        }
    }
}

class Test : public App {
public:
    Test() {
        auto& world = GetWorld();
        world
            .AddPlugins<DefaultPlugins>()
            .AddStartupSystem(LoadResourcesSystem)
            .AddSystem(UpdateSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)