#include "engine/sound/sound.hpp"

namespace engine {

Mix_Music* Sound::music_ = nullptr;

void Sound::Init() {
    Uint32 initFlag = MIX_INIT_OGG|MIX_INIT_MP3|MIX_INIT_MID;
    if (Mix_Init(initFlag) != initFlag) {
        Loge("SDL mixer init failed: {}", Mix_GetError());
    }
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) != 0) {
        Loge("open audio device failed: {}", Mix_GetError());
    }
}

void Sound::Quit() {
    Mix_CloseAudio();
    if (music_) {
        Mix_FreeMusic(music_);
    }
    Mix_Quit();
}

void Sound::PlaySound(const std::string& filename, int loop) {
    if (music_) {
        Mix_FreeMusic(music_);
    }
    music_ = Mix_LoadMUS(filename.c_str());
    if (!music_) {
        Loge("music {} load failed: {}", filename, Mix_GetError());
        Mix_PlayMusic(music_, loop);
    }
}

Sound::Sound(SoundID id, const std::string& filename, const std::string& name)
    : id_(id), name_(name), channel_(0) {
    chunk_ = Mix_LoadWAV(filename.c_str());
    if (!chunk_) {
        Loge("sound {} load failed: {}", filename, Mix_GetError());
    }
}

Sound::~Sound() {
    Mix_FreeChunk(chunk_);
}

void Sound::Play(int loops) {
    channel_ = Mix_PlayChannel(-1, chunk_, loops);
}

void Sound::Stop() {
    if (!IsStop()) {
        Mix_Pause(channel_);
    }
}

void Sound::SetPosition(const Vec2& position) {
    float len = Length(position);
    Mix_SetPosition(channel_, std::atan(position.x / position.y), len);
}

void Sound::FadeIn(int milliseconds) {
    Mix_FadeInChannel(-1, chunk_, 1, milliseconds);
}

void Sound::FadeOut(int milliseconds) {
    Mix_FadeOutChannel(channel_, milliseconds);
}

bool Sound::IsStop() const {
    return Mix_Paused(channel_);
}

void SoundFactory::Init() { }

void SoundFactory::Quit() {
    soundMap_.clear();
}

Sound* SoundFactory::Load(const std::string& filename, const std::string& name) {
    if (auto sound = Find(name); sound) {
        Logw("sound {} already loaded", name);
        return sound;
    } else {
        SoundID id = curID_++;
        auto s = std::make_unique<Sound>(id, filename, name);
        auto result = s.get();
        soundMap_[id] = std::move(s);
        return result;
    }
}

void SoundFactory::Remove(Sound* sound) {
    Remove(sound->ID());
}

void SoundFactory::Remove(SoundID id) {
    auto it = soundMap_.find(id);
    if (it != soundMap_.end()) {
        soundMap_.erase(it);
    }
}

void SoundFactory::Remove(const std::string& name) {
    auto sound = Find(name);
    if (!sound) return;
    Remove(sound->ID());
}

Sound* SoundFactory::Find(SoundID id) {
    auto it = soundMap_.find(id);
    if (it == soundMap_.end()) {
        return nullptr;
    } else {
        return it->second.get();
    }
}

Sound* SoundFactory::Find(const std::string& name) {
    for (auto& sound : soundMap_) {
        if (sound.second->Name() == name) {
            return sound.second.get();
        }
    }
    return nullptr;
}


SoundID SoundFactory::curID_ = 0;
std::unordered_map<SoundID, std::unique_ptr<Sound>> SoundFactory::soundMap_;

}