#pragma once
#include "pch.hpp"

namespace grogue::core {

class Timer final {
public:
    static Timer SteadyTimer;

    std::uint32_t GetMsBetweenFrames();
    double GetSecondsBetweenFrames();
    void Record();

private:
    std::uint32_t ms_ = 0; 
    std::chrono::steady_clock::time_point curTime_{};
};

}
