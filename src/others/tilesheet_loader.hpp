#pragma once

#include "engine/engine.hpp"
#include "path_helper.hpp"
#include "in_out.hpp"
#include "path_helper.hpp"

void LoadAllImageResources(const std::filesystem::path& path);
void LoadImage(const std::filesystem::path& root, const std::filesystem::path& filepath);
bool IsTileesheetConfigFileExists(const std::filesystem::path& tilesheet, OUT std::filesystem::path& outConfigFilename);
void LoadTilesheet(const std::filesystem::path& root,
                   const std::filesystem::path& tilesheetPath,
                   const std::filesystem::path& configPath);