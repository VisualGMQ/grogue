#include "app/app.hpp"
#include "app/animation.hpp"
#include "app/tilesheet.hpp"
#include "test_helper.hpp"

math::Rect rect{0, 0, 100, 100};
ClipInfo<float> xClip;
ClipInfo<float> yClip;
Tile tile;

struct PlayerGroup {
    AnimClipPlayer<float> xPlayer;
    AnimClipPlayer<float> yPlayer;
};

ClipInfo<Tile> imageClip;

void InitPropertyClipSystem(ecs::Commands& cmd, ecs::Resources resources) {
    xClip.clip = std::make_unique<AnimationClip<float>>(rect.x);
    xClip.clip->AppendFrame(CreateBasicPropFrame<float>(0, 0));
    xClip.clip->AppendFrame(CreateBasicPropFrame<float>(5000, 800));

    yClip.clip = std::make_unique<AnimationClip<float>>(rect.y);
    yClip.clip->AppendFrame(CreateBasicPropFrame<float>(0, 0));
    yClip.clip->AppendFrame(CreateBasicPropFrame<float>(3000, 500));

    PlayerGroup group = {
        AnimClipPlayer<float>(xClip),
        AnimClipPlayer<float>(yClip),
    };
    group.xPlayer.Play();
    group.yPlayer.Play();
    cmd.SetResource(std::move(group));
}

void LoadResourceSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& tilesheetManager = resources.Get<TileSheetManager>();
    auto path = TestHelper::Instance().GetResourcePath();
    imageClip.clip = std::make_unique<AnimationClip<Tile>>(tile);
    auto tilesheet = tilesheetManager.LoadFromConfig(path + "airman_desc.lua");
    imageClip.handle = tilesheet.Handle();
    for (int i = 0; i < 8; i++) {
        imageClip.clip->AppendFrame(CreateTileFrame(i * 1000, Tile{tilesheet.Get(0, i).region}));
    }

    AnimClipPlayer<Tile> imagePlayer(imageClip);
    imagePlayer.Play();
    cmd.SetResource<AnimClipPlayer<Tile>>(std::move(imagePlayer));
    cmd.SetResource<TileSheet>(std::move(tilesheet));
}

void UpdatePropSystem(ecs::Commands& cmd, ecs::Queryer queryer, ecs::Resources resources, ecs::Events& events) {
    auto& group = resources.Get<PlayerGroup>();
    auto& timer = resources.Get<Timer>();
    group.xPlayer.Update(timer);
    group.yPlayer.Update(timer);

    auto& renderer = resources.Get<Renderer>();
    renderer.SetDrawColor({0, 255, 0});
    renderer.DrawRect(rect);
}

void UpdateImageSystem(ecs::Commands& cmd, ecs::Queryer queryer, ecs::Resources resources, ecs::Events& events) {
    auto& spritePlayer = resources.Get<AnimClipPlayer<Tile>>();
    auto& timer = resources.Get<Timer>();
    auto& tilesheet = resources.Get<TileSheet>();
    spritePlayer.Update(timer);

    auto& renderer = resources.Get<Renderer>();
    Sprite sprite = Sprite::Default();
    sprite.region = tile.region;
    sprite.customSize.x = tile.region.w;
    sprite.customSize.y = tile.region.h;
    SpriteBundle bundle{sprite, tilesheet.Handle(), {}};
    renderer.DrawSprite(bundle);
}

class Test : public App {
public:
    Test() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
             .AddStartupSystem(InitPropertyClipSystem)
             .AddStartupSystem(LoadResourceSystem)
             .AddSystem(UpdatePropSystem)
             .AddSystem(UpdateImageSystem)
             .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)