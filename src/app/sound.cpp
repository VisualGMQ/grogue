#include "app/sound.hpp"

BGM::BGM(BGMHandle handle, const std::string& filename) : handle_(handle) {
    music_ = Mix_LoadMUS(filename.c_str());
    if (!music_) {
        LOGE("music ", filename, " load failed");
        BGMHandle::Destroy(handle);
    }
}

BGM::BGM(BGM&& bgm): handle_(BGMHandle::Null()) {
    swap(*this, bgm);
}

BGM& BGM::operator=(BGM&& o) {
    if (&o != this) {
        swap(o, *this);
    }

    return *this;
}

BGM::~BGM() {
    BGMHandle::Destroy(handle_);
    Mix_FreeMusic(music_);
}


BGMHandle BGMManager::Load(const std::string& filename) {
    auto handle = BGMHandle::Create();
    storeNewItem(handle, std::unique_ptr<BGM>(new BGM(handle, filename)));
    return handle;
}


void BGMPlayer::Play(BGMHandle handle, int loops) {
    if (manager_.Has(handle)) {
        auto& bgm = manager_.Get(handle);
        Mix_PlayMusic(bgm.music_, loops);
    } else {
        LOGW("BGM handle invalid");
    }
}

void BGMPlayer::FadeIn(BGMHandle handle, int ms, int loops) {
    if (manager_.Has(handle)) {
        auto& bgm = manager_.Get(handle);
        Mix_FadeInMusic(bgm.music_, loops, ms);
    } else {
        LOGW("BGM handle invalid");
    }
}

void BGMPlayer::FadeOut(int ms) {
    Mix_FadeOutMusic(ms);
}

void BGMPlayer::SetVolumn(float volumn) {
    Mix_VolumeMusic(volumn * MIX_MAX_VOLUME);
}

void BGMPlayer::Stop() {
    Mix_PauseMusic();
}