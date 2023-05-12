#pragma once

#include <string>
#include <fstream>
#include <optional>
#include "log.hpp"

//! @addtogroup utilities
//! @{

//! @brief read whole file content
//! @param filename 
//! @return file content. Read file failed will return `std::nullopt`
inline std::optional<std::string> ReadWholeFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.fail()) {
        LOGE("file %s open failed", filename);
        return std::nullopt;
    }
    std::string content(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
    return content;
}

//! @}