#pragma once

#include "pch.hpp"
#include "image.hpp"
#include "timer.hpp"
#include "tilesheet.hpp"

namespace grogue::core  {

struct Frame final {
    Frame(Image, uint32_t ms);

    Image image;
    uint32_t delayMs;
};

class Animation final {
public:
    static Animation Create(const std::initializer_list<Frame>& frames);
    static Animation Create(const TileSheet&,
                            const std::vector<unsigned int> milliseconds,
                            int row);

    Animation() = default;

    bool Empty() const { return frames_.empty(); }
    size_t Size() const { return frames_.size(); }
    auto begin() { return frames_.begin(); }
    auto end() { return frames_.end(); }
    void PushBack(Frame f) { frames_.push_back(f); }
    void Insert(Frame f, size_t idx) { frames_.insert(frames_.begin() + idx, f); }
    Frame& Get(size_t idx) { return frames_.at(idx); }
    Frame& operator[](size_t idx) { return frames_[idx]; }

    bool IsPlaying() const { return isPlaying_; }
    void Play() { isPlaying_ = true; }
    void Rewind() { curFrameIdx_ = 0; remainTime_ = 0; }
    void Pause() { isPlaying_ = false; }
    void Stop() { Pause(); Rewind(); }
    size_t GetCurFrameIdx() { return curFrameIdx_; }
    Frame& GetCurFrame() { return frames_[curFrameIdx_]; }
    void SetLoop(int count) { loopCount_ = count; }

    void Update();

private:
    std::vector<Frame> frames_;
    bool isPlaying_ = false;
    int loopCount_ = 0;
    size_t curFrameIdx_ = 0;
    std::uint32_t remainTime_ = 0;

    Animation(const std::initializer_list<Frame>& frames);
    Animation(const std::vector<Frame>& frames);
};

}
