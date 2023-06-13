#pragma once

#include "core/math.hpp"
#include "app/sprite.hpp"
#include "app/renderer.hpp"
#include "game/types.hpp"

enum class Direction: uint8_t {
    None = 0x00,
    Up = 0x01,
    Down = 0x02,
    Left = 0x04,
    Right = 0x08,
};

//! @brief component for all monster(include player)
struct LUA_BIND_COMPONENT Monster final {
    enum DirIdx {
        Right = 0,
        Up = 1,
        Down = 2,
    };

    SpriteBundle sprites[3];
    DirIdx curSpriteIdx = DirIdx::Down;
    Direction direction;

    math::Vector2 velocity; // TODO: move it to physics later
    math::Vector2 position; // TODO: move it to physics later

    static Monster CreateMonster(const SpriteBundle& right, const SpriteBundle& up, const SpriteBundle& down);

    void Move(const math::Vector2& vel);
};


void MonsterUpdate(ecs::Commands& cmd, ecs::Querier queryer,
                   ecs::Resources resources, ecs::Events& events);

void DrawMonsterSystem(ecs::Commands& cmd, ecs::Querier queryer,
                   ecs::Resources resources, ecs::Events& events);