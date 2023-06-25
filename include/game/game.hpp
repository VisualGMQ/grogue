#pragma once

#include "core/pch.hpp"
#include "app/app.hpp"
#include "game/config.hpp"
#include "game/map.hpp"
#include "game/monster.hpp"
#include "game/signal_defs.hpp"
#include "game/input.hpp"

struct LUA_BIND_RESOURCE NearestItemHover {
    bool valid = false;
    math::Vector2 position;
};

inline void LUA_BIND PlayerMove(const Input& input, Monster& monster, physic::Particle& particle) {
    constexpr float SPEED = 50;

    auto axis = input.Axis();
    particle.vel = math::Vector2(axis.x * SPEED, -axis.y * SPEED);

    monster.Move(particle.vel);
}
