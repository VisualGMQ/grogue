#pragma once

#include "app/fwd.hpp"
#include "app/sprite.hpp"
#include "app/timer.hpp"
#include "core/ecs.hpp"
#include "core/math.hpp"
#include "core/pch.hpp"

template <typename T>
inline T NoInterpolation(T a, T b, float t) {
    return a;
}

template <typename T>
using InterpFunc = std::function<T(T, T, float)>;

template <typename T>
struct Frame final {
public:
    template <typename U>
    friend class AnimPlayer;

    template <typename U>
    friend Frame<U> CreateBasicPropFrame(U, Timer::TimeType, InterpFunc<U>);

    friend Frame<Tile> CreateTileFrame(Timer::TimeType time, const Tile& tile);

    auto Duration() const { return time_; }

    const T& Value() const { return value_; }

private:
    Timer::TimeType time_;
    T value_;
    InterpFunc<T> interpolation_;
};

template <typename T>
Frame<T> CreateBasicPropFrame(T value, Timer::TimeType time,
                              InterpFunc<T> func = math::Lerp<T>) {
    Frame<T> frame;
    frame.time_ = time;
    frame.value_ = value;
    frame.interpolation_ = func;
    return frame;
}

template <typename T>
class AnimatedProperty final {
public:
    void SetFrames(const std::vector<Frame<T>>& frames) { frames_ = frames; }

    void AppendFrame(const Frame<T>& frame) { frames_.push_back(frame); }

    const std::vector<Frame<T>>& GetFrames() const { return frames_; }

    std::vector<Frame<T>>& GetFrames() { return frames_; }

    size_t FrameSize() const { return frames_.size(); }

    Frame<T>& operator[](size_t index) { return frames_[index]; }

    const Frame<T>& operator[](size_t index) const { return frames_[index]; }

    bool Empty() const { return frames_.empty(); }

private:
    std::vector<Frame<T>> frames_;
};

template <typename T>
class AnimPlayer final {
public:
    AnimPlayer(AnimatedProperty<T>& clip) : clip_(clip) {
        if (!clip_.Empty()) {
            property_ = clip_[0].Value();
        }
    }

    void Play() { isPlaying_ = true; }

    void Pause() { isPlaying_ = false; }

    void Stop() {
        isPlaying_ = false;
        clip_.curTime = 0;
        clip_.frameIndex = 0;
    }

    bool IsPlaying() const { return isPlaying_; }

    void Update(const Timer& timer) {
        if (!isPlaying_) {
            return;
        }

        if (frameIndex_ >= clip_.FrameSize() - 1) {
            property_ = clip_.GetFrames().back().value_;
            return;
        }

        curTime_ += timer.Elapse();
        auto& frame = clip_[frameIndex_];
        auto& nextFrame = clip_[frameIndex_ + 1];
        auto duration = nextFrame.time_ - frame.time_;
        if (curTime_ >= duration) {
            curTime_ -= duration;
            frameIndex_++;
        }

        property_ =
            frame.interpolation_(frame.value_, nextFrame.value_,
                                 static_cast<float>(curTime_) / duration);
    }

    T GetProp() const { return property_; }

private:
    T property_;
    AnimatedProperty<T>& clip_;
    Timer::TimeType curTime_ = 0;
    uint32_t frameIndex_ = 0;
    bool isPlaying_ = false;
};
