#include "app/image.hpp"

Image::Image(Renderer& renderer, const std::string& filename)
    : handle_(ImageHandle::Create()) {
    texture_ = IMG_LoadTexture(renderer.renderer_, filename.c_str());
    if (!texture_) {
        Assert(texture_ != nullptr,
               "texture " + filename + " load failed: " + IMG_GetError());
    }
}

Image::~Image() {
    SDL_DestroyTexture(texture_);
}
