#pragma once

#include "engine/core/vmath.hpp"
#include "engine/core/timer.hpp"

namespace engine {

template <typename T>
class KeyFrame final {
public:
    KeyFrame(T data, int time): data_(data), time_(time) { }
    int Time() const { return time_; }
    void SetTime(int time) { time_ = time; }
    T& Data() { return data_; }
    const T& Data() const { return data_; }

private:
    T data_;
    int time_;
};

template <typename T>
class AnimationBase {
public:
    AnimationBase(const std::initializer_list<KeyFrame<T>>& frames, T& attribute, bool shouldRevertFirstFrame = true);
    virtual ~AnimationBase() = default;

    void Play();
    void Pause();
    void Stop();
    void Rewind();
    bool IsPlay() const { return isPlay_; }
    bool ShouldRevertFirstFrame() const { return shouldRevertFirstFrame_; }
    void EnableRevertFirstFrame() { shouldRevertFirstFrame_ = true; }
    void DisableRevertFirstFrame() { shouldRevertFirstFrame_ = false; }

    void SetLoop(int loop) {
        loop_ = loop;
        curLoop_ = loop;
    }

    virtual bool WillInterpolate() const = 0;
    virtual void Update() = 0;

protected:
    struct KeyFrameOrderByTime {
        bool operator()(const KeyFrame<T>& f1, const KeyFrame<T>& f2) const {
            return f1.Time() < f2.Time();
        }
    };

    std::vector<KeyFrame<T>> keyframes_;
    bool isPlay_ = false;
    uint32_t curTime_ = 0;
    uint32_t curFrameIdx_ = 0;
    T& attribute_;
    bool shouldRevertFirstFrame_ = true;
    int loop_ = 0;
    int curLoop_ = 0;
};

template <typename T>
AnimationBase<T>::AnimationBase(const std::initializer_list<KeyFrame<T>>& frames, T& attribute, bool shouldRevertFirstFrame)
: attribute_(attribute), shouldRevertFirstFrame_(shouldRevertFirstFrame) {
    std::set<KeyFrame<T>, KeyFrameOrderByTime> keyframeSet(frames);
    for (auto& frame : keyframeSet) {
        keyframes_.push_back(frame);
    }
}

template <typename T>
void AnimationBase<T>::Play() {
    isPlay_ = true;
    Rewind();
}

template <typename T>
void AnimationBase<T>::Pause() {
    isPlay_ = false;
}

template <typename T>
void AnimationBase<T>::Stop() {
    Pause();
    Rewind();
}

template <typename T>
void AnimationBase<T>::Rewind() {
    curTime_ = 0;
    curFrameIdx_ = 0;
    attribute_ = keyframes_[0].Data();
    curLoop_ = loop_;
}

template <typename T, bool ShouldInterploate>
class Animation: public AnimationBase<T> {
public:
    Animation(const std::initializer_list<KeyFrame<T>>& frames, T& attribute, bool shouldRevertFirstFrame = true);
    void Update() override;
    bool WillInterpolate() const override { return ShouldInterploate; }
};

template <typename T>
class Animation<T, true>: public AnimationBase<T> {
public:
    Animation(const std::initializer_list<KeyFrame<T>>& frames, T& attribute, bool shouldRevertFirstFrame = true): AnimationBase<T>(frames, attribute, shouldRevertFirstFrame) {}
    void Update() override;
    bool WillInterpolate() const override { return true; }
};

template <typename T>
class Animation<T, false>: public AnimationBase<T> {
public:
    Animation(const std::initializer_list<KeyFrame<T>>& frames, T& attribute, bool shouldRevertFirstFrame = true): AnimationBase<T>(frames, attribute, shouldRevertFirstFrame) {}
    void Update() override;
    bool WillInterpolate() const override { return true; }
};

template <typename T>
void Animation<T, true>::Update() {
    if (!this->IsPlay()) return;

    this->curTime_ += Timer::GetElapse();

    while (this->curFrameIdx_ < this->keyframes_.size() && 
           this->keyframes_[this->curFrameIdx_].Time() < this->curTime_) {
        this->curFrameIdx_++;
    }

    if (this->curFrameIdx_ >= this->keyframes_.size()) {
        if (this->curLoop_ == -1 || this->curLoop_ > 0) {
            if (this->curLoop_ > 0) this->curLoop_ --;
            int tmpLoop = this->curLoop_;
            this->Rewind();
            this->curLoop_ = tmpLoop;
        } else {
            if (this->ShouldRevertFirstFrame()) {
                this->Stop();
            } else {
                this->Pause();
                this->attribute_ = this->keyframes_.back().Data();
            }
            return;
        }
    }

    uint32_t prevFrameIdx = this->curFrameIdx_ - 1;
    const T& curAttr = this->keyframes_[this->curFrameIdx_].Data();
    T prevAttr = this->curFrameIdx_ == 0 ? this->keyframes_[0].Data() : this->keyframes_[prevFrameIdx].Data();
    uint32_t curFrameTime = this->keyframes_[this->curFrameIdx_].Time(),
             prevFrameTime = this->curFrameIdx_ == 0 ? 0 : this->keyframes_[prevFrameIdx].Time();
    uint32_t deltaTime = curFrameTime - prevFrameTime;
    this->attribute_ = Lerp<T, T>(prevAttr, curAttr, (float)(this->curTime_ - prevFrameTime) / float(deltaTime));
}

template <typename T>
void Animation<T, false>::Update() {
    if (!this->IsPlay()) return;

    this->curTime_ += Timer::GetElapse();

    while (this->curFrameIdx_ < this->keyframes_.size() && 
           this->keyframes_[this->curFrameIdx_].Time() < this->curTime_) {
        this->curFrameIdx_++;
    }

    if (this->curFrameIdx_ >= this->keyframes_.size()) {
        if (this->curLoop_ == -1 || this->curLoop_ > 0) {
            if (this->curLoop_ > 0) this->curLoop_ --;
            int tmpLoop = this->curLoop_;
            this->Rewind();
            this->curLoop_ = tmpLoop;
        } else {
            if (this->ShouldRevertFirstFrame()) {
                this->Stop();
            } else {
                this->Pause();
                this->attribute_ = this->keyframes_.back().Data();
            }
            return;
        }
    }

    uint32_t prevFrameIdx = this->curFrameIdx_ - 1;
    const T& curAttr = this->keyframes_[this->curFrameIdx_].Data();
    T prevAttr = this->curFrameIdx_ == 0 ? this->keyframes_[0].Data() : this->keyframes_[prevFrameIdx].Data();
    uint32_t curFrameTime = this->keyframes_[this->curFrameIdx_].Time(),
             prevFrameTime = this->curFrameIdx_ == 0 ? 0 : this->keyframes_[prevFrameIdx].Time();
    uint32_t deltaTime = curFrameTime - prevFrameTime;
    this->attribute_ = this->curFrameIdx_ == 0 ? this->keyframes_[0].Data() : this->keyframes_[prevFrameIdx].Data();
}

}
