#include "map.hpp"

std::unique_ptr<Map> MapManager::groundMap_;
std::vector<std::unique_ptr<Map>> MapManager::dungeonMap_;
int MapManager::curDungeonMapIdx_ = 0;