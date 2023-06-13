#include "game/game.hpp"

void PlayerMove(Keyboard& keyboard, Monster& monster) {
    constexpr float SPEED = 2;

    math::Vector2 velocity;

    if (keyboard.Key(KEY_D).IsPressing() ||
        keyboard.Key(KEY_A).IsPressing()) {
        if (keyboard.Key(KEY_D).IsPressing()) {
            velocity.Set(1.0, 0.0);
        } else {
            velocity.Set(-1.0, 0.0);
        }
    }
    if (keyboard.Key(KEY_W).IsPressing() ||
        keyboard.Key(KEY_S).IsPressing()) {
        if (keyboard.Key(KEY_S).IsPressing()) {
            velocity += math::Vector2(0, 1);
        } else {
            velocity += math::Vector2(0, -1);
        }
    }

    velocity.Normalize();
    velocity *= SPEED;
    monster.Move(velocity);
}
