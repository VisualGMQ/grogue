#pragma once
#include "pch.hpp"

namespace grogue::core {

class Timer final {
public:
    static std::uint32_t GetMsBetweenFrames();
    static void Record();

private:
    static std::uint32_t ms_; 
    static std::chrono::steady_clock::time_point curTime_;
};

}
