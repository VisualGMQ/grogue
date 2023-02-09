#pragma once

#include "core/pch.hpp"

class Timer final {
public:
    static void UpdateSystem(ecs::Commands&, ecs::Queryer, ecs::Resources,
                             ecs::Events&);

    Timer();

    uint64_t Elapse() const { return elapse_; }

private:
    std::chrono::steady_clock::time_point curTime_;
    uint64_t elapse_;
};