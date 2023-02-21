#include "app/app.hpp"
#include "app/animation.hpp"
#include "app/tilesheet.hpp"
#include "test_helper.hpp"

std::unique_ptr<AnimatedProperty<int>> tileCol;
math::Rect rect = {0, 0, 50, 50};

struct PlayerGroup {
    AnimBunchPlayer<float> positionPlayer;
    AnimPlayer<float> rowPlayer;
};

void InitPropertyClipSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& luaManager = resources.Get<AssetsManager>().Lua();

    auto resourcePath = TestHelper::Instance().GetResourcePath();
    auto tileCol = LoadAnim(luaManager, resourcePath + "frame_animation.lua");
    auto clips = LoadAnims(luaManager, resourcePath + "rect_animation.lua");

    PlayerGroup group;
    AnimBunchPlayer<float> bunchPlayer;
    group.positionPlayer.SetAnims(clips);
    group.rowPlayer = tileCol;

    group.positionPlayer.Play();
    group.positionPlayer.SetLoop(AnimInfiniteLoop);
    group.rowPlayer.Play();
    group.rowPlayer.SetLoop(AnimInfiniteLoop);
    cmd.SetResource(std::move(group));
}

void LoadResourceSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& tilesheetManager = resources.Get<TileSheetManager>();

    cmd.SetResource<TileSheet>(std::move(tilesheetManager.LoadFromConfig(TestHelper::Instance().GetResourcePath() + "airman_desc.lua")));

}

void UpdatePropSystem(ecs::Commands& cmd, ecs::Querier queryer, ecs::Resources resources, ecs::Events& events) {
    auto& group = resources.Get<PlayerGroup>();
    auto& timer = resources.Get<Timer>();
    auto& tilesheet = resources.Get<TileSheet>();

    group.positionPlayer.Update(timer);
    group.rowPlayer.Update(timer);

    rect.x = group.positionPlayer.GetPlayer("x")->GetProp();
    rect.y = group.positionPlayer.GetPlayer("y")->GetProp();
    int col = group.rowPlayer.GetProp();

    auto& renderer = resources.Get<Renderer>();
    auto tile = tilesheet.Get(0, col);

    Sprite sprite = Sprite::Default();
    sprite.region = tile.region;
    sprite.customSize.x = tile.region.w;
    sprite.customSize.y = tile.region.h;
    SpriteBundle bundle{sprite, tile.handle, {}};

    renderer.SetDrawColor({0, 255, 0});
    renderer.DrawRect(rect);
    renderer.DrawSprite(bundle);
}

class Test : public App {
public:
    Test() {
        auto& world = GetWorld();
        world
            .AddPlugins<DefaultPlugins>()
            .AddStartupSystem(InitPropertyClipSystem)
            .AddStartupSystem(LoadResourceSystem)
            .AddSystem(UpdatePropSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)