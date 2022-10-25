#include "engine/core/timer.hpp"

namespace engine {

uint64_t Timer::sElapse_ = 0;
uint64_t Timer::sTime_ = 0;

uint64_t Timer::GetTime() {
    return SDL_GetTicks64();
}

uint64_t Timer::GetElapse() {
    return sElapse_;
}

void Timer::UpdateElapse() {
    sElapse_ = GetTime() - sTime_;
    sTime_ = GetTime();
}

Uint32 Timer::myCallback(Uint32 interval, void *param) {
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = param;
    userevent.data2 = nullptr;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

Timer::Timer(TimerID id, Callback callback, uint32_t interval, void* param): id_(id), callback_(callback), interval_(interval) {
    param_.owner = this;
    param_.userParam = param;
}

Timer::~Timer() {
    Stop();
}

void Timer::Start() {
    innerId_ = SDL_AddTimer(interval_, myCallback, (void*)&param_);
    if (innerId_ == 0) {
        Loge("Timer start failed: {}", SDL_GetError());
    }
}

void Timer::Stop() {
    SDL_RemoveTimer(innerId_);
}


std::unordered_map<SDL_TimerID, std::unique_ptr<Timer>> TimerFactory::timerMap_;
TimerID TimerFactory::curId_ = 0;

Timer* TimerFactory::Create(Timer::Callback callback, uint32_t interval, void* param) {
    TimerID id = curId_++;
    std::unique_ptr<Timer> timer = std::make_unique<Timer>(id, callback, interval, param);
    Timer* result = timer.get();
    timerMap_[id] = std::move(timer);
    return result;
}

Timer* TimerFactory::Find(TimerID) {
    if (auto it = timerMap_.begin(); it != timerMap_.end()) {
        return it->second.get();
    } else {
        return nullptr;
    }
}

void TimerFactory::Remove(TimerID id) {
    timerMap_.erase(id);
}

}