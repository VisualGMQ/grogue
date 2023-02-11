#include "app/app.hpp"
#include "app/animation.hpp"

math::Rect rect{0, 0, 100, 100};
ClipInfo<float> xClip;
ClipInfo<float> yClip;

struct PlayerGroup {
    AnimClipPlayer<float> xPlayer;
    AnimClipPlayer<float> yPlayer;
};

void LoadResourcesSystem(ecs::Commands& cmd, ecs::Resources resources) {
    xClip.clip = std::make_unique<AnimationClip<float>>(rect.x);
    xClip.clip->AppendFrame(Frame<float>{0, 0});
    xClip.clip->AppendFrame(Frame<float>{5000, 800});

    yClip.clip = std::make_unique<AnimationClip<float>>(rect.y);
    yClip.clip->AppendFrame(Frame<float>{0, 0});
    yClip.clip->AppendFrame(Frame<float>{3000, 500});

    PlayerGroup group = {
        AnimClipPlayer<float>(xClip),
        AnimClipPlayer<float>(yClip),
    };
    group.xPlayer.Play();
    group.yPlayer.Play();
    cmd.SetResource(std::move(group));
}

void UpdateSystem(ecs::Commands& cmd, ecs::Queryer queryer, ecs::Resources resources, ecs::Events& events) {
    auto& group = resources.Get<PlayerGroup>();
    auto& timer = resources.Get<Timer>();
    group.xPlayer.Update(timer);
    group.yPlayer.Update(timer);

    auto& renderer = resources.Get<Renderer>();
    renderer.SetDrawColor({0, 255, 0});
    renderer.DrawRect(rect);
}

class Test : public App {
public:
    Test() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
             .AddStartupSystem(LoadResourcesSystem)
             .AddSystem(UpdateSystem)
             .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)