#pragma once

#include "engine/core/pch.hpp"

namespace engine {


using TimerID = unsigned int;

class DLLEXPORT Timer final {
public:
    struct Param {
        void* userParam;
        Timer* owner;
    };

    using Callback = std::function<uint32_t(Timer&, uint32_t, void*)>;

    static uint64_t GetTime();
    static uint64_t GetElapse();
    static void UpdateElapse();

    Timer() = default;
    Timer(TimerID id, Callback, uint32_t, void*);
    ~Timer();
    void Update();
    TimerID ID() const { return id_; }

    uint32_t Interval() const { return interval_; }

    Callback GetCallback() const { return callback_; }

    void Start();
    void Stop();

private:
    Callback callback_;
    Param param_;
    uint32_t interval_;
    SDL_TimerID innerId_;
    TimerID id_;

    static Uint32 myCallback(Uint32 interval, void *param);

    static uint64_t sElapse_;
    static uint64_t sTime_;
};

class TimerFactory final {
public:
    TimerFactory() = delete;

    static void Init() {}
    static void Quit() { timerMap_.clear(); }
    DLLEXPORT static Timer* Create(Timer::Callback, uint32_t, void*);
    DLLEXPORT static Timer* Find(TimerID);
    DLLEXPORT static void Remove(TimerID);

private:
    static std::unordered_map<SDL_TimerID, std::unique_ptr<Timer>> timerMap_;
    static TimerID curId_;
};

}