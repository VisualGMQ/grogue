#include "data.hpp"

std::unique_ptr<GameData> GameData::instance_ = nullptr;

GameData* GameData::Instance() {
    if (!instance_) {
        instance_ = std::make_unique<GameData>();
    }
    return instance_.get();
}

void GameData::InitControllers(engine::Entity* player) {
    humanController_ = std::make_unique<HumanController>(player);
    backpackController_ = std::make_unique<BackpackController>();
    compositeController_ = std::make_unique<CompositeController>();
    controller_ = humanController_.get();
}

std::vector<engine::Entity*> MonsterManager::monsters_;
bool MonsterManager::shouldSort_ = true;
