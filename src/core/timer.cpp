#include "core/timer.hpp"

namespace grogue::core {

Timer Timer::SteadyTimer;

std::uint32_t Timer::GetMsBetweenFrames() {
    return ms_;
}

double Timer::GetSecondsBetweenFrames() {
    return GetMsBetweenFrames() / 1000.0f;
}

void Timer::Record() {
    auto t = std::chrono::steady_clock::now();
    ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(t - curTime_).count();
    curTime_ = t;
}

}
