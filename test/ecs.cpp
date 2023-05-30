#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "core/ecs.hpp"

using namespace ecs;

struct Name {
    std::string name;
};

struct ID {};

struct Male {};

struct Res{};

void SetupSystem(ecs::Commands& commands, ecs::Resources) {
    commands.Spawn<Name>(Name{"man1"});
    commands.Spawn<ID>(ID{});
    commands.SetResource<Res>(Res{});
}

void SpawnSystem(ecs::Commands& commands, ecs::Querier, ecs::Resources, ecs::Events&) {
    commands.Spawn<Male, Name>(Male{}, Name{"man2"});
}

TEST_CASE("example") {
    World world;
    world.AddStartupSystem(SetupSystem)
         .AddSystem(SpawnSystem);

    world.Startup();

    // you don't need to do this usually, because system has the querier as parameter
    Querier querier(world);

    auto entities = querier.Query<Name>();
    REQUIRE(entities.size() == 1);
    REQUIRE(querier.Has<Name>(entities[0]));
    REQUIRE_FALSE(querier.Has<ID>(entities[0]));
    auto& name = querier.Get<Name>(entities[0]);
    REQUIRE(name.name == "man1");

    entities = querier.Query<ID>();
    REQUIRE(entities.size() == 1);

    REQUIRE(querier.Has<ID>(entities[0]));
    REQUIRE_FALSE(querier.Has<Name>(entities[0]));

    // you don't need to do this usually, because system has the querier as parameter
    Commands cmd(world);

    /*
     * this will destroy after cmd.Execute()
     * (this will auto call after world.update() when you use cmd in system)
     */
    cmd.DestroyEntity(entities[0]);

    entities = querier.Query<ID>();
    REQUIRE(entities.size() == 1);

    // we call this handly, you shouldn't call it
    cmd.Execute();

    entities = querier.Query<ID>();
    REQUIRE(entities.empty());

    Resources res(world);
    REQUIRE(res.Has<Res>());

    world.Update();

    // template for new feature test
    entities = querier.Query<Name, Male>();
    ecs::CondQuerier condQuerier(world);
    REQUIRE(condQuerier.Has<ecs::All<Name, Male>>(entities[0]));
    // end

    entities = querier.Query<Name>();
    REQUIRE(entities.size() == 2);

    entities = querier.Query<Male>();
    REQUIRE(entities.size() == 1);

    Commands cmd2(world);
    cmd2.DestroyComponent<Male>(entities[0]);
    cmd2.AddComponent<ID>(entities[0], ID{});

    entities = querier.Query<Male>();
    REQUIRE(entities.size() == 1);

    cmd2.Execute();

    REQUIRE(querier.Has<ID>(entities[0]));

    entities = querier.Query<Male>();
    REQUIRE(entities.empty());

    entities = querier.Query<Name>();
    REQUIRE(entities.size() == 2);

    world.Shutdown();
}