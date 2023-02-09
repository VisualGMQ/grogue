#pragma once

#include "app/assets.hpp"
#include "app/fwd.hpp"
#include "app/input.hpp"
#include "app/renderer.hpp"
#include "app/timer.hpp"
#include "app/window.hpp"
#include "core/pch.hpp"

class ExitTrigger final {
public:
    bool ShouldExit() const { return shouldExit_; }

    void Exit() { shouldExit_ = true; }

    static void DetectExitSystem(ecs::Commands& cmd, ecs::Queryer queryer,
                                 ecs::Resources resources, ecs::Events& events);

private:
    bool shouldExit_ = false;
};

class DefaultPlugins : public ecs::Plugins {
public:
    void Build(ecs::World* world) override;
    void Quit(ecs::World* world) override;
};

class App {
public:
    ecs::World& GetWorld() { return world_; }

    void Run();

private:
    ecs::World world_;
};

#define RUN_APP(clazz)                \
    int main(int argc, char** argv) { \
        clazz app;                    \
        app.Run();                    \
        return 0;                     \
    }
