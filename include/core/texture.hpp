#pragma once

#include "core/pch.hpp"
#include "core/mathf.hpp"
#include "core/log.hpp"
#include "core/storage.hpp"
#include "core/sington.hpp"
#include "core/handle.hpp"

namespace grogue::core {

enum Flip {
    NoFlip = SDL_FLIP_NONE,
    Vertical = SDL_FLIP_VERTICAL,
    Horizontal = SDL_FLIP_HORIZONTAL,
};

class Texture final {
public:
    friend class Renderer;

    Texture(const Texture&) = delete;
    Texture(SDL_Surface*);
    Texture(const std::string& filename);
    ~Texture();

    void Recreate(SDL_Surface*);
    void Recreate(const std::string& filename);

    Texture& operator=(const Texture&) = delete;

    Size GetSize() const { return size_; }

    /* this funciton prepare for ImGui, you can't use it at other palce */
    SDL_Texture* GetRawTexture() const { return texture_; }

private:
    SDL_Texture* texture_;
    Size size_;
    std::string name_;
};

using TextureHandle = Handle<Texture>;

class TextureMgr final: public Sington<TextureMgr> {
public:
    static TextureHandle GetNull();

    TextureHandle Load(const std::string&);
    TextureHandle Load(const std::string& filename, const std::string& name);
    TextureHandle GetHandleByName(std::string_view name);

    Texture* GetTextureByHandle(const TextureHandle&);
    Texture* GetTextureByName(std::string_view name);

    void Destroy(const TextureHandle& handle);
    void Clear();

private:
    struct Data {
        Texture texture;
        bool valid;
        std::string name;

        Data(SDL_Surface* surface, const std::string& name)
            : texture(surface), valid(true), name(name) {}
        Data(const std::string& filename, const std::string& name)
            : texture(filename), valid(true), name(name) {}
        void Reset(const std::string& filename, const std::string& name) {
            texture.Recreate(filename);
            valid = true;
            this->name = name;
        }
    };
    std::vector<Data> datas_;
    std::vector<std::uint16_t> emptySlot_;

    HandleMgr<Texture> handleMgr_;

    ~TextureMgr();
};

}
