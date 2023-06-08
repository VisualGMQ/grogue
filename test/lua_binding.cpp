#include "core/pch.hpp"
#include "app/app.hpp"
#include "game/script.hpp"
#include "test_helper.hpp"

enum TestSignalFnc {
    Func1 = 0,
};

void InitLuaScript(ecs::Commands& cmds, ecs::Resources res) {
    auto& signalMgr = res.Get<SignalManager>();
    signalMgr.Regist(Func1, [](ecs::Commands&, ecs::Querier, ecs::Resources, ecs::Events&, void*){
        LOGT("signaled");
    });
    auto& luaMgr = res.Get<AssetsManager>().Lua();
    auto parent = cmds.SpawnAndReturn<Node>(Node{});
    auto child = cmds.SpawnAndReturn<Script, Node>(
        Script{luaMgr.CreateSolitary(TestHelper::Instance().GetResourcePath() +
                                     "lua_script.lua")},
        Node{});
    HierarchyBuilder builder(cmds, parent);
    builder.SetChilds({child});
}

class Test final : public App {
public:
    Test() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
            .AddStartupSystem(InitLuaScript)
            .AddSystem(RunScriptSystem)
            .AddSystem(HierarchyRunScriptSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)