#include "app/image.hpp"

Image::Image(Renderer& renderer, const std::string& filename)
    : handle_(ImageHandle::Create()) {
    texture_ = IMG_LoadTexture(renderer.renderer_, filename.c_str());
    if (!texture_) {
        Assert(texture_ != nullptr,
               "texture " + filename + " load failed: " + IMG_GetError());
    } else {
        SDL_QueryTexture(texture_, nullptr, nullptr, &w_, &h_);
    }
}

Image::Image(Image&& o): handle_(ImageHandle::Null()) {
    std::swap(*this, o);
}

Image& Image::operator=(Image&& img) {
    if (&img != this) {
        std::swap(*this, img);
    }
    return *this;
}

Image::~Image() {
    SDL_DestroyTexture(texture_);
}


ImageHandle ImageManager::Load(Renderer& renderer, const std::string& filename) {
    ImageHandle handle = ImageHandle::Create();
    auto item = std::unique_ptr<Image>(new Image(renderer, filename));
    storeNewItem(handle, item);
    return handle;
}