#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "app/signal.hpp"

TEST_CASE("refl") {
    ecs::World world;
    ecs::Commands cmds(world);
    ecs::Querier querier(world);
    ecs::Resources resouces(world);
    ecs::Events events;

    SignalManager mgr; 
    int a = 0;
    mgr.Regist("signal1", "func1", [&](ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&) {
        a ++;
    });

    REQUIRE(a == 0);

    mgr.Raise("func1", cmds, querier, resouces, events);
    REQUIRE(a == 1);
    mgr.Raise("func1", cmds, querier, resouces, events);
    REQUIRE(a == 2);

    mgr.Regist("signal1", "func2", [&](ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&) {
        a += 3;
    });
    mgr.Raise("func2", cmds, querier, resouces, events);
    REQUIRE(a == 5);

    mgr.RaiseCategory("signal1", cmds, querier, resouces, events);
    REQUIRE(a == 9);

    mgr.Remove("func2");
    mgr.Raise("func2", cmds, querier, resouces, events);
    REQUIRE(a == 9);
    mgr.RaiseCategory("signal1", cmds, querier, resouces, events);
    REQUIRE(a == 10);
}