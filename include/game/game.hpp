#pragma once

#include "core/pch.hpp"
#include "app/app.hpp"
#include "game/config.hpp"
#include "game/map.hpp"
#include "game/monster.hpp"
#include "game/signal_defs.hpp"
#include "game/game.hpp"

struct NearestItemHover {
    bool valid = false;
    math::Vector2 position;
};

void LUA_BIND ToggleBackpackUIPanel(ecs::Querier querier, ecs::Resources res);

void LUA_BIND PickupItemOnTile(Monster& monster, Backpack& backpack, ecs::Commands& cmd,
                      ecs::Querier querier, ecs::Resources resources,
                      ecs::Events& events);

void LUA_BIND PlayerMove(Keyboard& keyboard, Monster& monster);