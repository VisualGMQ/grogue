#include "app/animation.hpp"

/*
AnimationPlayer::AnimationPlayer(Animation& anim) : anim_(anim) {

}
void AnimationPlayer::Play() {
    isPlaying_ = true;
}

void AnimationPlayer::Update(const Timer& timer) {
    uint32_t elapse = timer.Elapse();
    if (!isPlaying_) return;

    updateProperty(timer, anim_.GetXClip());
    updateProperty(timer, anim_.GetYClip());
    updateProperty(timer, anim_.GetZClip());
}
*/