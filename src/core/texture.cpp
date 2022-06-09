#include "core/texture.hpp"
#include "core/renderer.hpp"
#include "core/video.hpp"

namespace grogue::core {

Texture::Texture(SDL_Surface* surface) {
    Recreate(surface);
}

Texture::Texture(const std::string& filename): name_(filename) {
    Recreate(filename);
}

void Texture::Recreate(SDL_Surface* surface) {
    if (texture_) {
        SDL_DestroyTexture(texture_);
    }

    name_ = "__Grogue_None";
    size_.Set(0, 0);

    texture_ = SDL_CreateTextureFromSurface(VideoMgr::GetMainVideo()->renderer->renderer_,
                                            surface);
    if (!texture_) {
        LOG_WARN("create texture from surface failed");
    } else {
        size_.w = surface->w;
        size_.h = surface->h;
    }
}

void Texture::Recreate(const std::string& filename) {
    if (texture_) {
        SDL_DestroyTexture(texture_);
    }

    texture_ = IMG_LoadTexture(VideoMgr::GetMainVideo()->renderer->renderer_,
                               filename.c_str());
    if (!texture_) {
        LOG_WARN("Load texture %s failed", filename);
        name_ = "__Grogue_None";
    } else {
        SDL_QueryTexture(texture_, nullptr, nullptr, &size_.w, &size_.h);
        name_ = filename;
    }
}

Texture::~Texture() {
    SDL_DestroyTexture(texture_);
}


/* TextureMgr */

TextureHandle TextureMgr::GetNull() {
    return GetInstance().handleMgr_.GetNull();
}

TextureHandle TextureMgr::Load(const std::string& filename) {
    return Load(filename, filename);
}

TextureHandle TextureMgr::Load(const std::string& filename,
                          const std::string& name) {
    if (emptySlot_.empty()) {
        datas_.emplace_back(filename, true, name);
        return handleMgr_.Create(datas_.size() - 1);
    } else {
        std::uint16_t slot = emptySlot_.back();
        emptySlot_.pop_back();
        datas_[slot].Reset(filename, name);
        return handleMgr_.Create(slot);
    }
}

TextureHandle TextureMgr::GetHandleByName(std::string_view name) {
    for (size_t i = 0; i < datas_.size(); i++) {
        if (datas_[i].name == name && datas_[i].valid) {
            return handleMgr_.Create(i);
        }
    }
    
    return handleMgr_.GetNull();
}

Texture* TextureMgr::GetTextureByHandle(const TextureHandle&) {

}

Texture* TextureMgr::GetTextureByName(std::string_view name) {

}

void TextureMgr::Destroy(const TextureHandle& handle) {
    if (datas_[handle.GetIndex()].valid) {
        datas_[handle.GetIndex()].valid = false;
        emptySlot_.push_back(handle.GetIndex());
    }
}

void TextureMgr::Clear() {
    datas_.clear();
    emptySlot_.clear();
}

TextureMgr::~TextureMgr() {
    Clear();
}

}
