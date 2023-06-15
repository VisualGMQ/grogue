#include "app/app.hpp"

using namespace physic;

template <int N>
struct Moveable { };

void StartupSystem(ecs::Commands& commands, ecs::Resources res) {
    commands.Spawn<CollideShape, Particle>(CollideShape{std::make_shared<AABB>(
        math::Vector2::Zero, math::Vector2{50, 50})},
        Particle::Create(math::Vector2(100, 200), 1.0));
    commands.Spawn<CollideShape, Particle, Moveable<0>>(
        CollideShape{std::make_shared<AABB>(math::Vector2::Zero,
                                            math::Vector2{50, 50})},
        Particle::Create(math::Vector2(400, 200), 1.0),
        {});
    commands.Spawn<CollideShape, Particle, Moveable<1>>(
        CollideShape{std::make_shared<Circle>(math::Vector2::Zero, 50)},
        Particle::Create(math::Vector2(800, 200), 2.0),
        {});
}

void UpdateSystem(ecs::Commands& commands, ecs::Querier querier, ecs::Resources res, ecs::Events&) {
    auto& mouse = res.Get<Mouse>();

    if (mouse.LeftBtn().IsPressing()) {
        auto entity = querier.Query<Moveable<0>>()[0];
        auto& p = querier.Get<Particle>(entity);
        p.pos = mouse.Position();
    }
    if (mouse.RightBtn().IsPressing()) {
        auto entity = querier.Query<Moveable<1>>()[0];
        auto& p = querier.Get<Particle>(entity);
        p.pos = mouse.Position();
    }
}

class Test : public App {
public:
    Test() {
        auto& world = GetWorld();
        world
            .AddPlugins<DefaultPlugins>()
            .AddStartupSystem(StartupSystem)
            .AddSystem(UpdateSystem)
            .AddSystem(UpdateParticleSystem)
            .AddSystem(DoCollideSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)