#pragma once

#include "core/pch.hpp"

namespace grogue::core {

class TextureManager {
public:
    bool Load(std::string file_name, std::string textureid, SDL_Renderer* renderer);
    void Draw(std::string textureid, int x, int y, int width, int height, SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void DrawFrame(std::string textureid, int x, int y, int width, int height, int current_row, int current_frame, SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
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

