#include "app/app.hpp"

using namespace physic;

template <int N>
struct Moveable { };

void StartupSystem(ecs::Commands& commands, ecs::Resources res) {
    commands.Spawn<CollideShape, Particle>(CollideShape{std::make_shared<AABB>(
        math::Vector2::Zero, math::Vector2{50, 50})},
        Particle::Create(math::Vector2(100, 200), 1.0));
    commands.Spawn<CollideShape, Particle>(CollideShape{std::make_shared<AABB>(
        math::Vector2::Zero, math::Vector2{25, 50})},
        Particle::Create(math::Vector2(200, 200), 1.0));
    commands.Spawn<CollideShape, Particle>(CollideShape{std::make_shared<AABB>(
        math::Vector2::Zero, math::Vector2{50, 25})},
        Particle::Create(math::Vector2(200, 400), 1.0));
    commands.Spawn<CollideShape, Particle>(CollideShape{std::make_shared<Circle>(
        math::Vector2{300, 25}, 20)},
        Particle::Create(math::Vector2(200, 400), 1.0));

    commands.Spawn<CollideShape, Particle, Moveable<0>>(
        CollideShape{std::make_shared<AABB>(math::Vector2::Zero,
                                            math::Vector2{50, 50})},
        Particle::Create(math::Vector2(400, 200), 1.0),
        {});
    commands.Spawn<CollideShape, Particle, Moveable<1>>(
        CollideShape{std::make_shared<Circle>(math::Vector2::Zero, 50)},
        Particle::Create(math::Vector2(800, 200), 2.0),
        {});

    auto& physicWorld = res.Get<PhysicWorld>();
    physicWorld.forceGenerators.push_back([](Particle& p, Time::TimeType){
        if (p.vel.LengthSqrd() > 0.000001) {
            constexpr float FrictionFactor = 5.0;
            p.force += -math::Normalize(p.vel) * 1.0 / p.massInv * FrictionFactor;
        }
    });
}

void UpdateSystem(ecs::Commands& commands, ecs::Querier querier, ecs::Resources res, ecs::Events&) {
    auto& key = res.Get<Keyboard>();

    auto entity = querier.Query<Moveable<0>>()[0];
    auto& p = querier.Get<Particle>(entity);
    if (key.Key(Key::KEY_A).IsPressing()) {
        p.force.x -= 10;
    }
    if (key.Key(Key::KEY_D).IsPressing()) {
        p.force.x += 10;
    }
    if (key.Key(Key::KEY_W).IsPressing()) {
        p.force.y -= 10;
    }
    if (key.Key(Key::KEY_S).IsPressing()) {
        p.force.y += 10;
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