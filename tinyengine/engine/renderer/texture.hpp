#pragma once

#include "engine/core/pch.hpp"
#include "engine/core/vmath.hpp"
#include "engine/renderer/image.hpp"

namespace engine {

using TextureID = int;

/* Please make sure your image format is RGBA8888 */
class Texture final {
public:
    friend class TextureFactory;
    friend class Renderer;

    Texture(SDL_Surface*);
    Texture(const Texture&) = delete;
    ~Texture();

    Texture& operator=(const Texture&) = delete;

    float Width() const { return size_.w; }
    float Height() const { return size_.h; }

    TextureID ID() const { return myId_; }
    Size GetSize() const { return size_; }

private:
    SDL_Texture* texture_ = nullptr;
    TextureID myId_ = -1;
    Size size_;

    int padding_;
};

class TextureFactory final {
public:
    static void Init();
    static void Quit();

    static Texture* Create(const std::string& filename);
    static Texture* CreateFromXpm(char** xpmData);

private:
    static std::vector<std::unique_ptr<Texture>> textures_;
    static TextureID curId_;
};

}
