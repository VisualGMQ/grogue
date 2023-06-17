#pragma once

#include "core/pch.hpp"
#include "app/app.hpp"
#include "game/config.hpp"
#include "game/map.hpp"
#include "game/monster.hpp"
#include "game/signal_defs.hpp"
#include "game/game.hpp"

struct LUA_BIND_RESOURCE NearestItemHover {
    bool valid = false;
    math::Vector2 position;
};

inline void LUA_BIND PlayerMove(Keyboard& keyboard, Monster& monster, physic::Particle& particle) {
    constexpr float SPEED = 2;

    if (keyboard.Key(KEY_D).IsPressing() ||
        keyboard.Key(KEY_A).IsPressing()) {
        if (keyboard.Key(KEY_D).IsPressing()) {
            particle.force.Set(10.0, 0.0);
        } else {
            particle.force.Set(-10.0, 0.0);
        }
    }
    if (keyboard.Key(KEY_W).IsPressing() ||
        keyboard.Key(KEY_S).IsPressing()) {
        if (keyboard.Key(KEY_S).IsPressing()) {
            particle.force.Set(0.0, 10.0);
        } else {
            particle.force.Set(0.0, -10.0);
        }
    }

    monster.Move(particle.vel);
}