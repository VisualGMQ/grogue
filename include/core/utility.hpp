#pragma once

#include <string>
#include <fstream>
#include <optional>
#include "log.hpp"

inline std::optional<std::string> ReadWholeFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.fail()) {
        LOGE("file %s open failed", filename);
        return std::nullopt;
    }
    std::string content(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
    return content;
}
