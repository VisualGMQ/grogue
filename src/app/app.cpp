#include "app/app.hpp"

void DefaultPlugins::Build(ecs::World* world) {}

void App::Run() {
    world_.Startup();
}