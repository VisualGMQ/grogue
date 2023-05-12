#pragma once

#include "core/pch.hpp"
#include "app/handle.hpp"
#include "app/manager.hpp"

class BGM;
class Sound;

using BGMHandle = Handle<BGM>;

//! @addtogroup resource-manager
//! @{

class BGM final {
public:
    friend class BGMManager;
    friend class BGMPlayer;

    BGM(const BGM&) = delete;
    BGM(BGM&&);
    ~BGM();

    BGM& operator=(const BGM&) = delete;
    BGM& operator=(BGM&&);
    
private:
    BGMHandle handle_;
    Mix_Music* music_;

    BGM(BGMHandle handle, const std::string& filename);

    friend void swap(BGM& lhs, BGM& rhs) {
        std::swap(lhs.music_, rhs.music_);
        std::swap(lhs.handle_, rhs.handle_);
    }
};

class BGMManager final : public ResourceManager<BGM> {
public:
    BGMHandle Load(const std::string& filename);
};

class BGMPlayer final {
public:
    BGMPlayer(BGMManager& manager) : manager_(manager) {}
    BGMPlayer(BGMPlayer&&) = default;

    void Play(BGMHandle, int loops = 0);
    void FadeIn(BGMHandle, int ms, int loops = 0);
    void FadeOut(int ms);
    void SetVolumn(float volumn);
    void Stop();

    BGMPlayer(const BGMPlayer&) = delete;
    BGMPlayer& operator=(const BGMPlayer&) = delete;
    BGMPlayer& operator=(BGMPlayer&&) = default;

private:
    BGMManager& manager_;
};

//! @}