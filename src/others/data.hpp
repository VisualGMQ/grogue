#pragma once

#include "scripts/human_controller.hpp"
#include "scripts/controller.hpp"

class GameData final {
public:
    static GameData* Instance();

    Controller* GetController() { return controller_.get(); }

    void ChangeController(std::unique_ptr<Controller>&& controller) {
        controller_ = std::move(controller);
        controller = nullptr;
    }

    void SetPlayer(engine::Entity* player) { player_ = player; }
    void ClearPlayer() { player_ = nullptr; }
    engine::Entity* GetPlayer() { return player_; }

    void SetPickableObjGridPos(const engine::Vec2& gridPos) { pickableObjGridPos = gridPos; }
    void ClearPickableObjGridPos() { pickableObjGridPos = std::nullopt; }
    const std::optional<engine::Vec2>& GetPickableObjGridPos() const { return pickableObjGridPos; }

private:
    std::unique_ptr<Controller> controller_ = nullptr;
    engine::Entity* player_ = nullptr;
    std::optional<engine::Vec2> pickableObjGridPos = std::nullopt;

    static std::unique_ptr<GameData> instance_;
};