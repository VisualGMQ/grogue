#pragma once

#include <filesystem>
#include <string>

std::string RemoveRootFromPath(const std::string& root, const std::string& path);
void CvtWindowsDelim2Unix(std::string& text);
std::string GetFilenameNoExt(const std::string& filename);