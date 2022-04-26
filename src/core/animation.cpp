#include "core/animation.hpp"

namespace grogue::core  {

Frame::Frame(Image image, unsigned int ms): image(image), delayMs(ms) {}


Animation Animation::Create(const std::initializer_list<Frame>& frames) {
    return Animation(frames);
}

Animation Animation::Create(const TileSheet& tilesheet,
                            const std::vector<unsigned int> milliseconds,
                            int row) {
    std::vector<Frame> frames;
    for (size_t i = 0; i < tilesheet.GetCol(); i++) {
        frames.emplace_back(tilesheet.Get(i, row), milliseconds[i]);
    }
    return Animation(frames);
}

Animation::Animation(const std::initializer_list<Frame>& frames): frames_(frames) {}

Animation::Animation(const std::vector<Frame>& frames): frames_(frames) {}

void Animation::Update() {
    if (frames_.empty()) return;
    if (IsPlaying()) {
        remainTime_ += Timer::GetMsBetweenFrames();
    }

    while (IsPlaying() && remainTime_ >= GetCurFrame().delayMs) {
        remainTime_ += Timer::GetMsBetweenFrames();
        if (remainTime_ > GetCurFrame().delayMs) {
            remainTime_ -= GetCurFrame().delayMs;
            curFrameIdx_ ++;
        }
        if (curFrameIdx_ >= frames_.size()) {
            if (loopCount_ == -1 || loopCount_ > 0) {
                Rewind();
                if (loopCount_ > 0) loopCount_ --;
            } else {
                Pause();
                curFrameIdx_ --;
                remainTime_ = 0;
            }
        }
    }
}

}
