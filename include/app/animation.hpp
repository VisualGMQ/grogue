#pragma once

#include "core/pch.hpp"
#include "core/ecs.hpp"
#include "app/timer.hpp"

template <typename T>
struct Frame final {
    Timer::TimeType time;
    T value;
    std::function<T(T, T, float)> interpolation = math::Lerp<T>;
};

template <typename T>
class AnimationClip final {
public:
    AnimationClip(T& prop): prop_(prop) {}

    void SetFrames(const std::vector<Frame<T>>& frames) {
        frames_ =  frames;
    }
    void AppendFrame(const Frame<T>& frame) {
        frames_.push_back(frame);
    }
    const std::vector<Frame<T>>& GetFrames() const {
        return frames_;
    }
    std::vector<Frame<T>>& GetFrames() {
        return frames_;
    }

    T& Prop() { return prop_; }

private:
    T& prop_;
    std::vector<Frame<T>> frames_;
};

template <typename T>
struct ClipInfo {
    Timer::TimeType curTime = 0;
    uint32_t frameIndex = 0;
    std::unique_ptr<AnimationClip<T>> clip = nullptr;

    void Reset() {
        curTime = 0;
        frameIndex = 0;
        clip = nullptr;
    }
};

class Animation final {
public:
    void SetPosXClip(const AnimationClip<float>& clip) {
        xClip_.Reset();
        xClip_.clip.reset(new AnimationClip<float>(clip));
    }

    void SetPosYClip(const AnimationClip<float>& clip) {
        yClip_.Reset();
        yClip_.clip.reset(new AnimationClip<float>(clip));
    }

    void SetPosZClip(const AnimationClip<float>& clip) {
        zClip_.Reset();
        zClip_.clip.reset(new AnimationClip<float>(clip));
    }

    ClipInfo<float>& GetXClip() { return xClip_; }

    ClipInfo<float>& GetYClip() { return yClip_; }

    ClipInfo<float>& GetZClip() { return zClip_; }

private:
    ClipInfo<float> xClip_;
    ClipInfo<float> yClip_;
    ClipInfo<float> zClip_;
};

template <typename T>
class AnimClipPlayer final {
public:
    AnimClipPlayer(ClipInfo<T>& clip): clip_(clip) {}

    void Play() {
        isPlaying_ = true;
    }

    void Update(const Timer& timer) {
        if (!isPlaying_) {
            return;
        }

        if (clip_.frameIndex >= clip_.clip->GetFrames().size() - 1) {
            clip_.clip->Prop() = clip_.clip->GetFrames().back().value;
            return ;
        }

        clip_.curTime += timer.Elapse();
        auto& frame = clip_.clip->GetFrames()[clip_.frameIndex];
        auto& nextFrame = clip_.clip->GetFrames()[clip_.frameIndex + 1];
        auto duration = nextFrame.time - frame.time;
        if (clip_.curTime >= duration) {
            clip_.curTime -= duration;
            clip_.frameIndex ++;
        }

        clip_.clip->Prop() = frame.interpolation(frame.value, nextFrame.value, static_cast<float>(clip_.curTime) / duration);
    }

private:
    ClipInfo<T>& clip_;
    bool isPlaying_ = false;
};

/*
class AnimationPlayer final {
public:
    AnimationPlayer(Animation&);

    void Play();

    void Update(const Timer&);

private:
    Animation& anim_;
    bool isPlaying_ = false;

};
*/