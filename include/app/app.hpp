#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "app/lua_manager.hpp"
#include "app/window.hpp"
#include "app/renderer.hpp"
#include "core/ecs.hpp"
#include "core/log.hpp"

class DefaultPlugins : public ecs::Plugins {
public:
    void Build(ecs::World* world) override;
    void Quit(ecs::World* world) override;
};

class App final {
public:
    ecs::World& GetWorld() { return world_; }

    void Run();

private:
    ecs::World world_;
    bool shouldClose_ = false;
};
