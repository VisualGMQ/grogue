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
#include "app/systems.hpp"
#include "app/scene.hpp"
#include "app/ui.hpp"
#include "app/assets.hpp"
#include "app/signal.hpp"
#include "app/physic.hpp"

//! @brief resources for sharing variable between lua
struct LUA_BIND_RESOURCE LuaShareContext final {
    sol::object context;
};

//! @brief resources for sharing variable between lua
struct LUA_BIND LuaEventContext final {
    sol::object context;
};


// a resource to trigger application exit
class LUA_BIND_RESOURCE ExitTrigger final {
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
    virtual ~App() = default;

    ecs::World& GetWorld() { return world_; }

    void Init();
    bool Run();
    void Shutdown();

private:
    ecs::World world_;
};

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

#define RUN_APP(clazz) \
void mainloop(void* param) { \
    clazz* app  = (clazz*)param; \
    static bool shouldClose = false; \
    if (!shouldClose) { \
        shouldClose = app->Run(); \
    } else { \
        LOGI("app shutdown"); \
    } \
} \
\
int main(int argc, char** argv) { \
    clazz app; \
    app.Init(); \
    emscripten_set_main_loop_arg(mainloop, &app, 0, 1); \
    app.Shutdown(); \
}

#else
#define RUN_APP(clazz)                \
    int main(int argc, char** argv) { \
        clazz app;                    \
        app.Init();                   \
        bool shouldClose = false;     \
        while (!shouldClose) {           \
            shouldClose = app.Run();  \
        }                             \
        app.Shutdown();               \
        return 0;                     \
    }
#endif