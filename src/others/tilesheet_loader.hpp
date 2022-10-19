#pragma once

#include "engine/engine.hpp"
#include "path_helper.hpp"

// for c-style code, point out whether param is IN or OUT(default is IN)
#define IN
#define OUT

void LoadImageResources(const std::filesystem::path& root, const std::filesystem::path& path);
void LoadImage(const std::filesystem::path& root, const std::filesystem::path& filepath);
bool ExistsTileesheetConfigFile(const std::filesystem::path& tilesheet, OUT std::filesystem::path& outConfigFilename);
void LoadTilesheet(const std::filesystem::path& root,
                   const std::filesystem::path& tilesheetPath,
                   const std::filesystem::path& configPath);
bool IsImageFile(const std::filesystem::path& filepath);
