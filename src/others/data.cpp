#include "data.hpp"

std::unique_ptr<GameData> GameData::instance_ = nullptr;

GameData* GameData::Instance() {
    if (!instance_) {
        instance_ = std::make_unique<GameData>();
    }
    return instance_.get();
}