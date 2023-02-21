#pragma once

#include "core/pch.hpp"

class Timer final {
public:
    using TimeType = uint64_t;
    static void UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources,
                             ecs::Events&);

    Timer();

    TimeType Elapse() const { return elapse_ > 0 ? elapse_ : 1; }

private:
    std::chrono::steady_clock::time_point curTime_;
    uint64_t elapse_;
};