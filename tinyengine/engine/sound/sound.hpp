#pragma once

#include "engine/core/vmath.hpp"

namespace engine {

using SoundID = unsigned int;

class Sound {
public:
    static void Init();
    static void Quit();
    static void PlaySound(const std::string& filename, int loop = 0);

    Sound(SoundID id, const std::string& filename, const std::string& name);
    ~Sound();
    void Play(int loops = 0);
    void Stop();
    void SetPosition(const Vec2& position);
    void FadeIn(int milliseconds);
    void FadeOut(int milliseconds);
    bool IsStop() const;
    void SetLoop(bool);
    const std::string& Name() const { return name_; }
    SoundID ID() const { return id_; }

private:
    static Mix_Music* music_;
    std::string name_;
    Mix_Chunk* chunk_;
    SoundID id_;
    std::uint32_t channel_;
};

class SoundFactory final {
public:
    SoundFactory() = delete;

    static void Init();
    static void Quit();

    static Sound* Load(const std::string& filename, const std::string& name);
    static void Remove(Sound*);
    static void Remove(SoundID);
    static void Remove(const std::string& name);
    static Sound* Find(SoundID);
    static Sound* Find(const std::string& name);

private:
    static SoundID curID_;
    static std::unordered_map<SoundID, std::unique_ptr<Sound>> soundMap_;
};

}