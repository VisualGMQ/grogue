#pragma once

#include "scripts/human_controller.hpp"
#include "scripts/controller.hpp"
#include "scripts/backpack_controller.hpp"

class GameData final {
public:
    static GameData* Instance();

    void InitControllers(engine::Entity* player);

    Controller* GetController() { return controller_; }
    HumanController* GetHumanController() { return humanController_.get(); }
    BackpackController* GetBackpackController() { return backpackController_.get(); }

    void ChangeController(Controller* controller) {
        controller_ = controller;
    }

    void SetPlayer(engine::Entity* player) { player_ = player; }
    void ClearPlayer() { player_ = nullptr; }
    engine::Entity* GetPlayer() { return player_; }

    void SetPickableObjGridPos(const engine::Vec2& gridPos) { pickableObjGridPos = gridPos; }
    void ClearPickableObjGridPos() { pickableObjGridPos = std::nullopt; }
    const std::optional<engine::Vec2>& GetPickableObjGridPos() const { return pickableObjGridPos; }

    void SetBackpackPanel(engine::Entity* backpack) { backpackPanel_ = backpack; }
    engine::Entity* GetBackpackPanel() { return backpackPanel_; }

private:
    Controller* controller_ = nullptr;
    std::unique_ptr<HumanController> humanController_ = nullptr;
    std::unique_ptr<BackpackController> backpackController_ = nullptr;
    engine::Entity* player_ = nullptr;
    engine::Entity* backpackPanel_ = nullptr;
    std::optional<engine::Vec2> pickableObjGridPos = std::nullopt;

    static std::unique_ptr<GameData> instance_;
};