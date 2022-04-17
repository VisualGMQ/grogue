#pragma once

#include "core/pch.hpp"
#include "core/renderer.hpp"
#include "core/mathf.hpp"
#include "core/log.hpp"

namespace grogue::core {

enum Flip {
    NoFlip = SDL_FLIP_NONE,
    Vertical = SDL_FLIP_VERTICAL,
    Horizontal = SDL_FLIP_HORIZONTAL,
};

class TextureManager {
public:
    bool Load(std::string file_name,
              std::string textureid,
              Renderer& renderer);
    void Draw(std::string textureid,
              int x, int y, int width, int height,
              Renderer& renderer,
              Flip flip = Flip::NoFlip);
    void DrawFrame(std::string textureid,
                   int x, int y, int width, int height,
                   int current_row,
                   int current_frame,
                   Renderer& renderer,
                   Flip flip = Flip::NoFlip);
    void ClearFromTextureMap(std::string textureid);

    static TextureManager& Instance() {
        static TextureManager instance;
        return instance;
    }

private:
    TextureManager();
    ~TextureManager();
    std::unordered_map<std::string, SDL_Texture*> texture_map_;
};

}

