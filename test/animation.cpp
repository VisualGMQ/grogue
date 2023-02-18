#include "app/app.hpp"
#include "app/animation.hpp"
#include "app/tilesheet.hpp"
#include "test_helper.hpp"

std::unique_ptr<AnimatedProperty<int>> tileCol;
math::Rect rect = {0, 0, 50, 50};

struct PlayerGroup {
    AnimBunchPlayer<float> positionPlayer;
    AnimPlayer<int> rowPlayer;
};

void InitPropertyClipSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto xClip = CreateAnimClip<float>();
    xClip->AppendFrame(CreateBasicPropFrame<float>(0, 0));
    xClip->AppendFrame(CreateBasicPropFrame<float>(800, 5000));

    auto yClip = CreateAnimClip<float>();
    yClip->AppendFrame(CreateBasicPropFrame<float>(0, 0));
    yClip->AppendFrame(CreateBasicPropFrame<float>(500, 3000));

    auto tileCol = CreateAnimClip<int>();
    tileCol->AppendFrame(CreateBasicPropFrame<int>(0, 0));
    tileCol->AppendFrame(CreateBasicPropFrame<int>(1, 500));
    tileCol->AppendFrame(CreateBasicPropFrame<int>(2, 1000));
    tileCol->AppendFrame(CreateBasicPropFrame<int>(3, 1500));
    tileCol->AppendFrame(CreateBasicPropFrame<int>(4, 2000));
    tileCol->AppendFrame(CreateBasicPropFrame<int>(5, 2500));
    tileCol->AppendFrame(CreateBasicPropFrame<int>(6, 3000));
    tileCol->AppendFrame(CreateBasicPropFrame<int>(7, 3500));

    PlayerGroup group;
    AnimBunchPlayer<float> bunchPlayer;
    auto xClipPlayer = CreateAnimPlayer<float>(xClip);
    auto yClipPlayer = CreateAnimPlayer<float>(yClip);

    group.positionPlayer.AddAnimPlayer(xClipPlayer);
    group.positionPlayer.AddAnimPlayer(yClipPlayer);
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

void UpdatePropSystem(ecs::Commands& cmd, ecs::Queryer queryer, ecs::Resources resources, ecs::Events& events) {
    auto& group = resources.Get<PlayerGroup>();
    auto& timer = resources.Get<Timer>();
    auto& tilesheet = resources.Get<TileSheet>();

    group.positionPlayer.Update(timer);
    group.rowPlayer.Update(timer);

    rect.x = group.positionPlayer.GetPlayer(0)->GetProp();
    rect.y = group.positionPlayer.GetPlayer(1)->GetProp();
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