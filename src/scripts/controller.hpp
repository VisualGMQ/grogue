#pragma once
#include "engine/engine.hpp"

class Command {
public:
    virtual ~Command() = default;
    virtual void Execute() = 0;
};

class WalkCommand: public Command {
public:
    WalkCommand(engine::Vec2& ownerVel,
                engine::Entity* hair, engine::Entity* head, engine::Entity* body, engine::Entity* feet,
                engine::Animation<engine::ImageInfo, false>* anim, engine::Node2DComponent* transform,
                int tilesheetRow);
    void SetVelocity(const engine::Vec2& velocity);
    void Execute() override;

private:
    engine::Entity* hair_;
    engine::Entity* head_;
    engine::Entity* body_;
    engine::Entity* feet_;
    engine::Vec2 velocity_;
    engine::Vec2& ownerVel_;
    engine::Animation<engine::ImageInfo, false>* anim_;
    engine::Node2DComponent* transform_;
    engine::TileSheet* tilesheet_;
    int tilesheetRow_;
};

class PlayerController final {
public:
    PlayerController(engine::Entity* entity,
                     engine::Entity* hair, engine::Entity* head, engine::Entity* body, engine::Entity* feet,
                     engine::Entity* weapon,
                     float speed, int tilesheetRow);
    void Update();

private:
    engine::Entity* entity_;
    engine::Entity* hair_;
    engine::Entity* head_;
    engine::Entity* body_;
    engine::Entity* feet_;
    engine::Entity* weapon_;
    float speed_;
    engine::Vec2 vel_;
    engine::Vec2 dir_;
    int tilesheetRow_;

    std::unique_ptr<WalkCommand> walk_;
    std::unique_ptr<engine::Animation<engine::ImageInfo, false>> walkAnim_;

    engine::TileSheet* tilesheet_;
};
