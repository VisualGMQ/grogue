#include "app/timer.hpp"

Timer::Timer() : elapse_(1) {
    curTime_ = std::chrono::steady_clock::now();
}

void Timer::UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources resources,
                         ecs::Events&) {
    if (resources.Has<Timer>()) {
        auto& timer = resources.Get<Timer>();
        timer.elapse_ = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - timer.curTime_)
                            .count();
        timer.curTime_ = std::chrono::steady_clock::now();
    }
}