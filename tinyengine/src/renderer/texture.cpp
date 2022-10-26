#include "engine/renderer/texture.hpp"
#include "engine/core/video.hpp"

namespace engine {

Texture::Texture(SDL_Surface* surface) {
    texture_ = SDL_CreateTextureFromSurface(Video::GetRenderer(), surface);
    if (!texture_) {
        Loge("create texture from surface failed: {}", SDL_GetError());
    } else {
        size_.w = surface->w;
        size_.h = surface->h;
    }
    SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
}

Texture::~Texture() {
    SDL_DestroyTexture(texture_);
}

std::vector<std::unique_ptr<Texture>> TextureFactory::textures_;
TextureID TextureFactory::curId_ = 0;

void TextureFactory::Init() {}

void TextureFactory::Quit() {
    textures_.clear();
}

Texture* TextureFactory::Create(const std::string& filename) {
    int channel, w, h;
    SDL_Surface* surface = IMG_Load(filename.data());
    textures_.push_back(std::make_unique<Texture>(surface));
    auto& texture = textures_.back();
    SDL_FreeSurface(surface);
    texture->myId_ = curId_++;
    return texture.get();
}

Texture* TextureFactory::CreateFromXpm(char** xpmData) {
    SDL_Surface* surface = IMG_ReadXPMFromArray(xpmData);
    textures_.push_back(std::make_unique<Texture>(surface));
    auto& texture = textures_.back();
    SDL_FreeSurface(surface);
    texture->myId_ = curId_++;
    return texture.get();
}

}
