#include "core/texture.hpp"

namespace grogue::core {
TextureManager::TextureManager() { }
TextureManager::~TextureManager() { }

bool TextureManager::Load(std::string file_name,
                          std::string textureid,
                          Renderer& renderer) {
    SDL_Surface* sur = IMG_Load(file_name.c_str());
    if (sur == nullptr) {
        LOG_ERROR("%s load failed", file_name.c_str());
        return false;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer.renderer_, sur);
    SDL_FreeSurface(sur);
    if (tex == nullptr) {
        LOG_ERROR("create %s texture failed", file_name.c_str());
        return false;
    }
    texture_map_[textureid] = tex;
    return true;
}

void TextureManager::Draw(std::string textureid,
                          int x, int y, int width, int height,
                          Renderer& renderer,
                          Flip flip) {
    Rect dst_rect(x, y, width, height);

    SDL_RenderCopyExF(renderer.renderer_,
                      texture_map_[textureid],
                      nullptr, &dst_rect.sdlrect,
                      0, 0,
                      static_cast<SDL_RendererFlip>(flip));
}

void TextureManager::DrawFrame(std::string textureid,
                               int x, int y, int width, int height,
                               int current_row,
                               int current_frame,
                               Renderer& renderer,
                               Flip flip) {

    SDL_Rect src_rect;
    src_rect.x = width * current_frame;  // current frame x coordinate
    src_rect.y = height * (current_row - 1);
    src_rect.w = width;
    src_rect.h = height;
    Rect dst_rect(x, y, width, height);

    SDL_RenderCopyExF(renderer.renderer_,
                      texture_map_[textureid],
                      &src_rect, &dst_rect.sdlrect,
                      0, 0,
                      static_cast<SDL_RendererFlip>(flip));
}

void TextureManager::ClearFromTextureMap(std::string textureid) {
    SDL_DestroyTexture(texture_map_[textureid]);
    texture_map_.erase(textureid);
}

}
