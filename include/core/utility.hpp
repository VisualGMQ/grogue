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

class Random final: public Singleton<Random, true> {
public:
    Random(): seed_(time(nullptr)) { Rand(); }
    explicit Random(int seed): seed_(seed) { Rand(); }

    std::uint32_t Rand() {
        num_ = (a_ * num_ + b_) % m_;
        return num_;
    }

    std::uint32_t RandRange(int low, int high) {
        return Rand() % (high - low) + low;
    }

    std::uint32_t Seed() const { return seed_; }

private:
    std::uint32_t a_ = 48271, b_ = 0, m_ = (1 << 31) - 1;
    std::uint32_t seed_;
    std::uint32_t num_;
};

//! @}