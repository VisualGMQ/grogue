#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "app/signal.hpp"

namespace test {

enum SignalCategory {
    Signal1 = 0,
};

enum SignalFunc {
    Func1 = 0,
    Func2,
};

}

TEST_CASE("signal") {
    ecs::World world;
    ecs::Commands cmds(world);
    ecs::Querier querier(world);
    ecs::Resources resouces(world);
    ecs::Events events;

    SignalManager mgr; 
    int a = 0;
    mgr.Regist(test::Signal1, test::Func1, [&](ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&, void*) {
        a ++;
    });

    REQUIRE(a == 0);

    mgr.Raise(test::Func1, cmds, querier, resouces, events, nullptr);
    REQUIRE(a == 1);
    mgr.Raise(test::Func1, cmds, querier, resouces, events, nullptr);
    REQUIRE(a == 2);

    mgr.Regist(test::Signal1, test::Func2, [&](ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&, void*) {
        a += 3;
    });
    mgr.Raise(test::Func2, cmds, querier, resouces, events, nullptr);
    REQUIRE(a == 5);

    mgr.RaiseCategory(test::Signal1, cmds, querier, resouces, events, nullptr);
    REQUIRE(a == 9);

    mgr.Remove(test::Func2);
    mgr.Raise(test::Func2, cmds, querier, resouces, events, nullptr);
    REQUIRE(a == 9);
    mgr.RaiseCategory(test::Signal1, cmds, querier, resouces, events, nullptr);
    REQUIRE(a == 10);
}