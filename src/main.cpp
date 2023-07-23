#include "app/app.hpp"
#include "game/config.hpp"
#include "game/game.hpp"
#include "game/input.hpp"
#include "game/map.hpp"
#include "game/monster.hpp"
#include "game/signal_defs.hpp"
#include "script/script.hpp"

void StartupSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& luaMgr = resources.Get<AssetsManager>().Lua();
    cmd.Spawn<Script>(Script::Create(luaMgr, "./resources/script/player.lua"));
}

class GameApp final : public App {
public:
    GameApp() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
            .AddStartupSystem(StartupSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(GameApp)