#pragma once

#include "core/pch.hpp"
#include "app/manager.hpp"

using TimeType = uint64_t;

//! @brief a resource that make you get time elapse between two frame
class [[refl, luabind(res)]] Time final {
public:
    static void UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources,
                             ecs::Events&);
    static void SetFPS(uint32_t fps) {
        fps_ = fps;
        fpsDuration_ = 1000.0 / fps;
    }
    static uint32_t GetFPS() { return fps_; }

    Time();
    [[luanobind]] Time(const Time&) = delete;
    Time(Time&&) = default;
    [[luanobind]] Time& operator=(const Time&) = delete;
    Time& operator=(Time&&) = default;

    //! @brief get elapse time between two frame (in millisecond)
    //! @return elapsed milliseconds
    TimeType Elapse() const { return elapse_ > 0 ? elapse_ : 1; }

private:
    static uint64_t fps_;
    static uint64_t fpsDuration_;

    uint64_t curTime_;
    uint64_t elapse_;
};


//! @addtogroup resource-manager
//! @{

class Timer;
using TimerHandle = Handle<Timer>;

using TickFunc = std::function<void()>;

//! @brief a timer that tirge function when reach time
class [[refl, luabind]] Timer final {
public:

    Timer(TimerHandle handle, TimeType time, TickFunc func, int loop = 0) : handle_(handle), dstTime_(time), func_(func), loop_(loop) { }

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    void OnTick(TickFunc func) {
        func_ = func;
    }

    void Update(const Time& t) {
        if (!isTicking_) return;

        curTime_ += t.Elapse();
        if (curTime_ > dstTime_) {
            curTime_ = 0; 
            if (func_) {
                func_();
            }
            if (loop_ > 0) {
                loop_--; 
            }
            if (loop_ == 0) {
                Stop(); 
            }
        }
    }

    void Start() { isTicking_ = true; }

    void Pause() {
        isTicking_ = false;
        curTime_ = 0;
     }

    void Stop() { isTicking_ = false; }

private:
    Handle<Timer> handle_;
    TickFunc func_ = nullptr;
    int loop_ = 0;
    bool isTicking_ = false;
    TimeType curTime_{};
    TimeType dstTime_;
};

class [[refl, luabind(res)]] TimerManager : public ResourceManager<Timer> {
public:
    static void UpdateSystem(ecs::Commands&, ecs::Querier, ecs::Resources,
                        ecs::Events&);

    TimerHandle Create(TimeType time, TickFunc func);
};

//! @}