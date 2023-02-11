#include "app/app.hpp"
#include "app/animation.hpp"
#include "test_helper.hpp"

math::Rect rect{0, 0, 100, 100};
ClipInfo<float> xClip;
ClipInfo<float> yClip;
ImageHandle imageHandle;

struct PlayerGroup {
    AnimClipPlayer<float> xPlayer;
    AnimClipPlayer<float> yPlayer;
};

ClipInfo<ImageHandle> imageClip;

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
    auto& imageManager = resources.Get<AssetsManager>().Image();
    auto path = TestHelper::Instance().GetResourcePath();
    imageClip.clip = std::make_unique<AnimationClip<ImageHandle>>(imageHandle);
    for (int i = 0; i < 5; i++) {
        auto handle = imageManager.Load(path + "airman" + std::to_string(i + 1) + ".png");
        imageClip.clip->AppendFrame(CreateImageFrame(i * 1000, handle));
    }

    AnimClipPlayer<ImageHandle> imagePlayer(imageClip);
    imagePlayer.Play();
    cmd.SetResource<AnimClipPlayer<ImageHandle>>(std::move(imagePlayer));
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
    auto& imagePlayer = resources.Get<AnimClipPlayer<ImageHandle>>();
    auto& timer = resources.Get<Timer>();
    imagePlayer.Update(timer);

    auto& renderer = resources.Get<Renderer>();
    Sprite sprite = Sprite::Default();
    SpriteBundle bundle{sprite, imageHandle, {}};
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