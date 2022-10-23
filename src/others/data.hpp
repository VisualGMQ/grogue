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

private:
    std::unique_ptr<Controller> controller_ = nullptr;

    static std::unique_ptr<GameData> instance_;
};