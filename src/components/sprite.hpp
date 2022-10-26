#pragma once

#include "engine/engine.hpp"

namespace component {

class Sprite: public engine::Component {
public:
    Sprite(engine::ComponentID id): engine::Component(id) { Reset(); }

    void Reset() override;

    engine::Image image;
    engine::Vec2 anchor;
    engine::Vec2 offset;
    engine::Vec2 size;
    float rotation;
};

void DrawSprite(Sprite* sprite, const engine::Transform&);

}