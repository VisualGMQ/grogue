#include "app/animation.hpp"

void AnimationPlayer::Play() {
    for (auto& player : basicPropPlayer_) {
        player->Play();
    }
}

void AnimationPlayer::Pause() {
    for (auto& player : basicPropPlayer_) {
        player->Pause();
    }
}

void AnimationPlayer::Stop() {
    for (auto& player : basicPropPlayer_) {
        player->Stop();
    }
}

void AnimationPlayer::Update(const Timer& timer) {
    for (auto& player : basicPropPlayer_) {
        player->Update(timer);
    }
}