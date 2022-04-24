#include "core/texture.hpp"
#include "core/renderer.hpp"
#include "core/video.hpp"

namespace grogue::core {

Texture::Texture(SDL_Surface* surface): size_(0, 0) {
    texture_ = SDL_CreateTextureFromSurface(VideoMgr::GetMainVideo()->renderer->renderer_,
                                            surface);
    if (!texture_) {
        LOG_WARN("create texture from surface failed");
    } else {
        size_.w = surface->w;
        size_.h = surface->h;
    }
}

Texture::Texture(const char* filename) {
    texture_ = IMG_LoadTexture(VideoMgr::GetMainVideo()->renderer->renderer_,
                               filename);
    if (!texture_) {
        LOG_WARN("Load texture %s failed", filename);
    } else {
        SDL_QueryTexture(texture_, nullptr, nullptr, &size_.w, &size_.h);
    }
}

Texture::~Texture() {
    SDL_DestroyTexture(texture_);
}


Storage<std::string, std::unique_ptr<Texture>> TextureMgr::storage_;

Texture* TextureMgr::Load(const std::string& filename) {
    return Load(filename, filename);
}

Texture* TextureMgr::Load(const std::string& filename,
                          const std::string& name) {
    if (auto texture = storage_.Find(name)) {
        return texture->get();
    } else {
        return storage_.Create(name,
                               std::make_unique<Texture>(filename.c_str())).get();
    }
}

Texture* TextureMgr::Find(const std::string& name) {
    auto texture = storage_.Find(name);
    if (!texture) {
        return nullptr;
    } else {
        return texture->get();
    }
}

void TextureMgr::Clear() {
    storage_.Clear();
}

}
