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

    Frame<T>& At(size_t index) {
        Assert(index < frames_.size(), "out of range");
        return (*this)[index];
    }

    const Frame<T>& At(size_t index) const {
        return const_cast<AnimatedProperty&>(*this).At(index);
    }

    bool Empty() const { return frames_.empty(); }

private:
    std::vector<Frame<T>> frames_;
};

template <typename T>
using AnimatedClip = std::shared_ptr<AnimatedProperty<T>>;

template <typename T>
AnimatedClip<T> CreateAnimClip() {
    return std::make_shared<AnimatedProperty<T>>();
}

static constexpr int AnimInfiniteLoop = -1;

template <typename T>
class AnimPlayer final {
public:
    template <typename U>
    friend class AnimBunchPlayer;

    AnimPlayer() = default;

    AnimPlayer(AnimatedClip<T> clip) : clip_(clip) {
        if (!clip_->Empty()) {
            property_ = clip_->At(0).Value();
        }
    }

    void SetClip(AnimatedClip<T> clip) {
        clip_ = clip;
    }

    void Play() { isPlaying_ = true; }

    void Pause() { isPlaying_ = false; }

    void Stop() {
        isPlaying_ = false;
        Rewind();
    }

    void Rewind() {
        curTime_ = 0;
        frameIndex_ = 0;
    }

    void SetLoop(int loopCount) {
        loop_ = loopCount;
    }

    bool IsPlaying() const { return isPlaying_; }

    void Update(const Timer& timer) {
        if (!isPlaying_) {
            return ;
        }

        if (isReachedLatestFrame()) {
            property_ = clip_->GetFrames().back().value_;

            if (loop_ != 0) {
                Rewind();
                if (loop_ > 0) {
                    loop_ --;
                }
            } else {
                isPlaying_ = false;
            }
            return;
        }

        curTime_ += timer.Elapse();
        auto& frame = clip_->At(frameIndex_);
        auto& nextFrame = clip_->At(frameIndex_ + 1);
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
    AnimatedClip<T> clip_;
    Timer::TimeType curTime_ = 0;
    uint32_t frameIndex_ = 0;
    bool isPlaying_ = false;
    int loop_ = 0;

    bool isReachedLatestFrame() const {
        return frameIndex_ >= clip_->FrameSize() - 1;
    }
};

template <typename T>
using AnimPlayerPtr = std::shared_ptr<AnimPlayer<T>>;

template <typename T>
AnimPlayerPtr<T> CreateAnimPlayer(AnimatedClip<T> clip) {
    return std::make_shared<AnimPlayer<T>>(clip);
}

template <typename T>
class AnimBunchPlayer final {
public:
    void AddAnimPlayer(AnimPlayerPtr<T> player) {
        players_.push_back(player);
        player->SetLoop(0);
    }

    void Play() {
        for (auto& player : players_) {
            player->Play();
        }
    }

    void Pause() {
        for (auto& player : players_) {
            player->Pause();
        }
    }

    void Stop() {
        for (auto& player : players_) {
            player->Stop();
        }
    }

    void Rewind() {
        for (auto& player : players_) {
            player->Rewind();
        }
    }

    bool IsPlaying() const {
        for (auto& player : players_) {
            if (player->IsPlaying()) {
                return true;
            }
        }
        return false;
    }

    void Update(const Timer& timer) {
        if (!IsPlaying()) {
            return;
        }

        if (isReachedLatestFrame()) {
            if (loop_ != 0) {
                Rewind();
                Play();
                if (loop_ > 0) {
                    loop_ --;
                }
            } else {
                return;
            }
        }

        for (auto& player : players_) {
            player->Update(timer);
        }
    }

    AnimPlayerPtr<T> GetPlayer(size_t index) { return players_.at(index); }

    void SetLoop(int loop) {
        loop_ = loop;
    }

private:
    std::vector<AnimPlayerPtr<T>> players_;
    int loop_ = 0;

    bool isReachedLatestFrame() const {
        for (auto& player : players_) {
            if (!player->isReachedLatestFrame()) {
                return false;
            }
        }
        return true;
    }
};
