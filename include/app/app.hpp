#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "app/lua_manager.hpp"
#include "app/window.hpp"
#include "app/renderer.hpp"
#include "app/image.hpp"
#include "app/input.hpp"
#include "core/ecs.hpp"
#include "core/log.hpp"
#include "app/fwd.hpp"

class DefaultPlugins : public ecs::Plugins {
public:
    void Build(ecs::World* world) override;
    void Quit(ecs::World* world) override;
};

class App {
public:
    App();

    ecs::World& GetWorld() { return world_; }
    void Run();

private:
    ecs::World world_;
};
