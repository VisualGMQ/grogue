#pragma once

#include "app/fwd.hpp"
#include "app/sprite.hpp"
#include "app/timer.hpp"
#include "core/ecs.hpp"
#include "core/math.hpp"
#include "core/pch.hpp"

inline Tile TileInterpolation(Tile a, Tile b, float t) {
    return a;
}

template <typename T>
using InterpFunc = std::function<T(T, T, float)>;

template <typename T>
struct Frame final {
public:
    template <typename U>
    friend class AnimClipPlayer;

    template <typename U>
    friend Frame<U> CreateBasicPropFrame(Timer::TimeType, U, InterpFunc<U>);

    friend Frame<Tile> CreateTileFrame(Timer::TimeType time, const Tile& tile);

private:
    Timer::TimeType time_;
    T value_;
    InterpFunc<T> interpolation_;
};

template <typename T>
Frame<T> CreateBasicPropFrame(Timer::TimeType time, T value,
                              InterpFunc<T> func = math::Lerp<T>) {
    Frame<T> frame;
    frame.time_ = time;
    frame.value_ = value;
    frame.interpolation_ = func;
    return frame;
}

inline Frame<Tile> CreateTileFrame(Timer::TimeType time,
                                   const Tile& tile) {
    Frame<Tile> frame;
    frame.time_ = time;
    frame.value_  = tile;
    frame.interpolation_ = TileInterpolation;
    return frame;
}

template <typename T>
class AnimationClip final {
public:
    AnimationClip(T& prop) : prop_(prop) {}

    void SetFrames(const std::vector<Frame<T>>& frames) { frames_ = frames; }

    void AppendFrame(const Frame<T>& frame) { frames_.push_back(frame); }

    const std::vector<Frame<T>>& GetFrames() const { return frames_; }

    std::vector<Frame<T>>& GetFrames() { return frames_; }

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

template <>
struct ClipInfo<Tile> {
    Timer::TimeType curTime = 0;
    uint32_t frameIndex = 0;
    std::unique_ptr<AnimationClip<Tile>> clip = nullptr;
    ImageHandle handle;

    void Reset() {
        curTime = 0;
        frameIndex = 0;
        clip = nullptr;
        handle = ImageHandle::Null();
    }
};

using ImageClip = ClipInfo<Sprite>;

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

    ClipInfo<float>& GetXClip() { return xClip_; }

    ClipInfo<float>& GetYClip() { return yClip_; }

private:
    ClipInfo<float> xClip_;
    ClipInfo<float> yClip_;
};

template <typename T>
class AnimClipPlayer final {
public:
    AnimClipPlayer(ClipInfo<T>& clip) : clip_(clip) {}

    void Play() { isPlaying_ = true; }

    void Pause() { isPlaying_ = false; }

    void Stop() {
        isPlaying_ = false;
        clip_.curTime = 0;
        clip_.frameIndex = 0;
    }

    void Update(const Timer& timer) {
        if (!isPlaying_) {
            return;
        }

        if (clip_.frameIndex >= clip_.clip->GetFrames().size() - 1) {
            clip_.clip->Prop() = clip_.clip->GetFrames().back().value_;
            return;
        }

        clip_.curTime += timer.Elapse();
        auto& frame = clip_.clip->GetFrames()[clip_.frameIndex];
        auto& nextFrame = clip_.clip->GetFrames()[clip_.frameIndex + 1];
        auto duration = nextFrame.time_ - frame.time_;
        if (clip_.curTime >= duration) {
            clip_.curTime -= duration;
            clip_.frameIndex++;
        }

        clip_.clip->Prop() =
            frame.interpolation_(frame.value_, nextFrame.value_,
                                 static_cast<float>(clip_.curTime) / duration);
    }

private:
    ClipInfo<T>& clip_;
    bool isPlaying_ = false;
};

class AnimationPlayer final {
public:
    enum BasicPropIndex {
        PosX = 0,
        PosY,
        ScaleX,
        ScaleY,
        Rotation,

        BasicPropNum,
    };

    void Play();
    void Pause();
    void Stop();
    void Update(const Timer&);

    AnimClipPlayer<float>& GetPropPlayer(BasicPropIndex prop) {
        return *basicPropPlayer_[prop];
    }

    void SetPropPlayer(BasicPropIndex prop,
                       std::unique_ptr<AnimClipPlayer<float>>&& clip) {
        basicPropPlayer_[prop] = std::move(clip);
    }

    AnimClipPlayer<Sprite>& GetSpritePlayer() { return *spritePlayer_; }

    void SetSpritePlayer(std::unique_ptr<AnimClipPlayer<Sprite>>&& clip) {
        spritePlayer_ = std::move(clip);
    }

private:
    std::array<std::unique_ptr<AnimClipPlayer<float>>, 5> basicPropPlayer_;
    std::unique_ptr<AnimClipPlayer<Sprite>> spritePlayer_;
};
