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

void LUA_BIND PlayerMove(Keyboard& keyboard, Monster& monster);