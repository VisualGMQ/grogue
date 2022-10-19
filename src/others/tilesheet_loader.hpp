#pragma once

#include "engine/engine.hpp"
#include <filesystem>

// for c-style code, point out whether param is IN or OUT(default is IN)
#define IN
#define OUT

std::string GetFilenameNoExt(const std::string& filename);

void LoadImageResources(const std::filesystem::path& root, const std::filesystem::path& path);
void LoadImage(const std::filesystem::path& root, const std::filesystem::path& filepath);
bool ExistsTileesheetConfigFile(const std::filesystem::path& tilesheet, OUT std::filesystem::path& outConfigFilename);
void LoadTilesheet(const std::filesystem::path& root,
                   const std::filesystem::path& tilesheetPath,
                   const std::filesystem::path& configPath);
bool IsImageFile(const std::filesystem::path& filepath);
std::string RemoveRootFromPath(const std::string& root, const std::string& path);
void CvtWindowsDelim2Unix(std::string& text);
