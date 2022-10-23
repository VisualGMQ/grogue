#pragma once

#include "controller.hpp"
#include "components/sprite.hpp"
#include "components/human.hpp"
#include "components/backpack.hpp"
#include "others/data.hpp"
#include "map/map.hpp"

class HumanController: public Controller {
public:
    HumanController(engine::Entity* entity);

    void Walk(const engine::Vec2& pos) override;
    void Pickup();
    void Update() override;

private:
    engine::Entity* entity_ = nullptr;
    engine::Node2DComponent* node2d_ = nullptr;
    component::Sprite* sprite_ = nullptr;
    component::Human* human_ = nullptr;
    component::Backpack* backpack_ = nullptr;
};