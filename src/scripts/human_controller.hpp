#pragma once

#include "controller.hpp"
#include "components/sprite.hpp"
#include "components/human.hpp"

class HumanController: public Controller {
public:
    HumanController(engine::Entity* entity);

    void Walk(const engine::Vec2& pos) override;
    void Update() override;

private:
    engine::Entity* entity_ = nullptr;
    engine::Node2DComponent* node2d_ = nullptr;
    component::Sprite* sprite_ = nullptr;
    component::Human* human_ = nullptr;
};