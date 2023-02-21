#pragma once

#include "app/assets.hpp"
#include "app/fwd.hpp"
#include "app/input.hpp"
#include "app/renderer.hpp"
#include "app/timer.hpp"
#include "app/window.hpp"
#include "app/tilesheet.hpp"
#include "app/image.hpp"
#include "core/pch.hpp"

// a resource to trigger application exit
class ExitTrigger final {
public:
    bool ShouldExit() const { return shouldExit_; }

    void Exit() { shouldExit_ = true; }

    static void DetectExitSystem(ecs::Commands& cmd, ecs::Querier querier,
                                 ecs::Resources resources, ecs::Events& events);

private:
    bool shouldExit_ = false;
};

// a resource to register a extra custom event handler
class ExtraEventHandler final {
public:
    using EventHandler = std::function<void(const SDL_Event&)>;
    using PrepareFunc = std::function<void(ecs::Resources)>;
    using FinishFunc = std::function<void(ecs::Resources)>;

    ExtraEventHandler(EventHandler handler, PrepareFunc prepare = [](ecs::Resources){},
                      FinishFunc finish = [](ecs::Resources){})
        : eventHandler_(handler), prepare_(prepare), finish_(finish) {}

    void Prepare(ecs::Resources resources) {
        prepare_(resources);
    }

    void Finish(ecs::Resources resources) {
        finish_(resources);
    }

    void HandleEvent(const SDL_Event& event) {
        eventHandler_(event);
    }

private:
    EventHandler eventHandler_;
    PrepareFunc prepare_;
    FinishFunc finish_;
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
