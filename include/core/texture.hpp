#pragma once

#include "core/pch.hpp"
#include "core/mathf.hpp"
#include "core/log.hpp"
#include "core/storage.hpp"

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
    Texture(const char* filename);
    ~Texture();

    Texture& operator=(const Texture&) = delete;

    Size GetSize() const { return size_; }

private:
    SDL_Texture* texture_;
    Size size_;
};

class TextureMgr final {
public:
    TextureMgr() = delete;

    static Texture* Load(const std::string&);
    static Texture* Load(const std::string& filename, const std::string& name);
    static Texture* Find(const std::string& name);
    static void Clear();
    
private:
    static Storage<std::string, std::unique_ptr<Texture>> storage_;
};

}
