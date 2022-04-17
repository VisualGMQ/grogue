#include "core/texture.hpp"

namespace grogue::core {
TextureManager::TextureManager() { }
TextureManager::~TextureManager() { }

bool TextureManager::Load(std::string file_name, std::string textureid, SDL_Renderer* renderer) {
    SDL_Surface* sur = IMG_Load(file_name.c_str());
    if (sur == nullptr) {
        return false;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sur);
    SDL_FreeSurface(sur);
    if (tex == nullptr) {
        return false;
    }
    texture_map_[textureid] = tex;
    return true;
}

void TextureManager::Draw(std::string textureid, int x, int y, int width, int height, SDL_Renderer* renderer, SDL_RendererFlip flip) {
    SDL_Rect dst_rect;
    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.w = width;
    dst_rect.h = height;

    SDL_RenderCopyEx(renderer, texture_map_[textureid], nullptr, &dst_rect, 0, 0, flip);
}

void TextureManager::DrawFrame(std::string textureid, int x, int y, int width, int height, int current_row, int current_frame, SDL_Renderer* renderer, SDL_RendererFlip flip) {
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    src_rect.x = width * current_frame;  // current frame x coordinate
    src_rect.y = height * (current_row - 1);
    src_rect.w = dst_rect.w = width;
    src_rect.h = dst_rect.h = height;
    dst_rect.x = x;
    dst_rect.y = y;

    SDL_RenderCopyEx(renderer, texture_map_[textureid], &src_rect, &dst_rect, 0, 0, flip);
}

void TextureManager::ClearFromTextureMap(std::string textureid) {
    SDL_DestroyTexture(texture_map_[textureid]);
    texture_map_.erase(textureid);
}

}
