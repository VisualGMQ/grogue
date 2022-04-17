#include "core/timer.hpp"

namespace grogue::core {

std::uint32_t Timer::ms_ = 0; 
std::chrono::steady_clock::time_point Timer::curTime_{};

std::uint32_t Timer::GetMsBetweenFrames() {
    return ms_;
}

void Timer::Record() {
    auto t = std::chrono::steady_clock::now();
    ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(t - curTime_).count();
    curTime_ = t;
}

}
