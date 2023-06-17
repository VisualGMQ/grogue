#include "game/monster.hpp"

void Monster::Move(const math::Vector2& vel) {
    uint8_t dir = static_cast<uint8_t>(vel.x > 0 ? Direction::Right : (vel.x < 0 ? Direction::Left : Direction::None));
    dir |= static_cast<uint8_t>(vel.y > 0 ? Direction::Down: (vel.y < 0 ? Direction::Up: Direction::None));

    if (dir & static_cast<uint8_t>(Direction::Up)) {
        curSpriteIdx = DirIdx::Up;
    }
    if (dir & static_cast<uint8_t>(Direction::Down)) {
        curSpriteIdx = DirIdx::Down;
    }
    if (dir & static_cast<uint8_t>(Direction::Left)) {
        curSpriteIdx = DirIdx::Right;
        sprites[DirIdx::Right].flip =Flip::Horizontal;
    } else {
        sprites[DirIdx::Right].flip = Flip::None;
    }
    if (dir & static_cast<uint8_t>(Direction::Right)) {
        curSpriteIdx = DirIdx::Right;
    }

    direction = static_cast<Direction>(dir);
}

Monster Monster::CreateMonster(const SpriteBundle& right, const SpriteBundle& up, const SpriteBundle& down) {
    Monster monster { {right, up, down}, DirIdx::Down, Direction::Down};
    return monster;
}

void DrawMonsterSystem(ecs::Commands& cmd, ecs::Querier queryer,
                   ecs::Resources resources, ecs::Events& events) {
    auto& renderer = resources.Get<Renderer>();

    auto entities = queryer.Query<ecs::With<Monster, physic::Particle>>();
    for (auto entity : entities) {
        auto& monster = queryer.Get<Monster>(entity);
        auto& particle = queryer.Get<physic::Particle>(entity);
        Transform transform = Transform::Create(particle.pos, 0, math::Vector2(SCALE, SCALE));
        renderer.DrawSprite(monster.sprites[monster.curSpriteIdx], transform);
    }
}
