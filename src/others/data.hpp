#pragma once

#include "controller/controller.hpp"
#include "controller/backpack_controller.hpp"
#include "controller/composite_controller.hpp"
#include "controller/human_controller.hpp"
#include "others/camera.hpp"

class HumanController;

class GameData final {
public:
    static GameData& Instance();

    void InitControllers(engine::Entity* player);

    void SetCamera(std::unique_ptr<Camera>&& camera) { camera_ = std::move(camera); }
    Camera& GetCamera() { return *camera_; }

    controller::Controller* GetController() { return controller_; }
    HumanController* GetHumanController() { return humanController_.get(); }
    BackpackController* GetBackpackController() { return backpackController_.get(); }
    CompositeController* GetCompositeController() { return compositeController_.get(); }

    void ChangeController(controller::Controller* controller) { controller_ = controller; }

    void SetPlayer(engine::Entity* player) { player_ = player; }
    void ClearPlayer() { player_ = nullptr; }
    engine::Result<engine::Entity*> GetPlayer() {
        if (player_) return engine::Ok(player_);
        return engine::Err{};
    }

    void SetPickableObjGridPos(const engine::Vec2& gridPos) { pickableObjGridPos_ = gridPos; }
    void ClearPickableObjGridPos() { pickableObjGridPos_ = std::nullopt; }
    const std::optional<engine::Vec2>& GetPickableObjGridPos() const { return pickableObjGridPos_; }

    void SetBackpackPanel(engine::Entity* backpack) { backpackPanel_ = backpack; }
    engine::Result<engine::Entity*> GetBackpackPanel() {
        if (backpackPanel_) return engine::Ok(backpackPanel_);
        return engine::Err{};
    }

    void SetCompositePanel(engine::Entity* composite) { compositePanel_ = composite; }
    engine::Result<engine::Entity*> GetCompositePanel() {
        if (compositePanel_) return engine::Ok(compositePanel_);
        return engine::Err{};
    }

    void SetCompositeDescriptionPanel(engine::Entity* composite) { compositeDescriptionPanel_ = composite; }
    engine::Result<engine::Entity*> GetCompositeDescriptionPanel() {
        if (compositeDescriptionPanel_) return engine::Ok(compositeDescriptionPanel_);
        return engine::Err{};
    }

    void SetLeftHandObjectFrame(engine::Entity* frame) { leftHandObjectFrame_ = frame; }
    void SetRightHandObjectFrame(engine::Entity* frame) { rightHandObjectFrame_ = frame; }
    engine::Result<engine::Entity*> GetLeftHandObjectFrame() {
        if (leftHandObjectFrame_) return engine::Ok(leftHandObjectFrame_);
        return engine::Err{};
    }
    engine::Result<engine::Entity*> GetRightHandObjectFrame() {
        if (rightHandObjectFrame_) return engine::Ok(rightHandObjectFrame_);
        return engine::Err{};
    }

    void SetPutTargetGridPos(const engine::Vec2& pos) { putTargetGridPos_ = pos; }
    const engine::Vec2& GetPutTargetGridPos() const {
        return putTargetGridPos_;
    }

    engine::Result<engine::Entity*> GetBackpackHoverObject();

private:
    controller::Controller* controller_ = nullptr;
    std::unique_ptr<HumanController> humanController_ = nullptr;
    std::unique_ptr<BackpackController> backpackController_ = nullptr;
    std::unique_ptr<CompositeController> compositeController_ = nullptr;
    engine::Entity* player_ = nullptr;
    engine::Entity* backpackPanel_ = nullptr;
    engine::Entity* compositePanel_ = nullptr;
    engine::Entity* compositeDescriptionPanel_ = nullptr;
    engine::Entity* leftHandObjectFrame_ = nullptr;
    engine::Entity* rightHandObjectFrame_ = nullptr;
    std::optional<engine::Vec2> pickableObjGridPos_ = std::nullopt;
    engine::Vec2 putTargetGridPos_;
    std::unique_ptr<Camera> camera_;

    static std::unique_ptr<GameData> instance_;
};

class MonsterManager {
public:
    using iterator = std::vector<engine::Entity*>::iterator;
    using const_iterator = std::vector<engine::Entity*>::const_iterator;

    static iterator begin() { return monsters_.begin(); }
    static iterator end() { return monsters_.end(); }
    static const_iterator cbegin() { return monsters_.cbegin(); }
    static const_iterator cend() { return monsters_.cend(); }

    static void Add(engine::Entity* monster) {
        monsters_.push_back(monster);
        shouldSort_ = true;
    }
    static void Clear() { monsters_.clear(); }
    static void Remove(engine::Entity* monster) {
        auto it = std::find(monsters_.begin(), monsters_.end(), monster);
        if (it != monsters_.end()) {
            monsters_.erase(it);
        }
    }

    static void SortMonsterInYAxis() {
        if (!shouldSort_) return;

        std::sort(monsters_.begin(), monsters_.end(),
        [](const engine::Entity* e1, const engine::Entity* e2) {
                auto node1 = e1->GetComponent<engine::Node2DComponent>();
                auto node2 = e2->GetComponent<engine::Node2DComponent>();

                if (node1.IsErr() || node2.IsErr()) return false;
                return node1.Unwrap()->position.y < node2.Unwrap()->position.y;
            });

        shouldSort_ = false;
    }

private:
    static std::vector<engine::Entity*> monsters_;
    static bool shouldSort_;
};
