#pragma once

#include "core/ecs.hpp"
#include "core/log.hpp"

class DefaultPlugins : public ecs::Plugins {
public:
    void Build(ecs::World* world) override;
};

class App final {
public:
    ecs::World& GetWorld() { return world_; }

    void Run();

private:
    ecs::World world_;
};
