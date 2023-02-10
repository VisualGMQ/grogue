#include "app/image.hpp"

Image::Image(ImageHandle handle, Renderer& renderer, const std::string& filename)
    : handle_(handle) {
    texture_ = IMG_LoadTexture(renderer.renderer_, filename.c_str());
    if (!texture_) {
        LOGE("texture " + filename + " load failed: " + IMG_GetError());
        ImageHandle::Destroy(handle_);
    } else {
        SDL_QueryTexture(texture_, nullptr, nullptr, &w_, &h_);
    }
}

Image::Image(ImageHandle handle, SDL_Texture* texture) : handle_(handle) {
    texture_ = texture;
    SDL_QueryTexture(texture_, nullptr, nullptr, &w_, &h_);
}

Image& Image::operator=(Image&& img) {
    if (&img != this) {
        std::swap(*this, img);
    }
    return *this;
}

Image::Image(Image&& o) : handle_(ImageHandle::Null()) {
    std::swap(*this, o);
}

Image::~Image() {
    ImageHandle::Destroy(handle_);
    SDL_DestroyTexture(texture_);
}

ImageManager::ImageManager(Renderer& renderer) {
    renderer_ = &renderer;
}

ImageHandle ImageManager::Load(const std::string& filename) {
    ImageHandle handle = ImageHandle::Create();
    storeNewItem(handle, std::unique_ptr<Image>(
                             new Image(handle, *renderer_, filename)));
    return handle;
}

std::unique_ptr<Image> ImageManager::CreateSolitary(SDL_Texture* texture) {
    return std::unique_ptr<Image>(new Image(ImageHandle::Null(), texture));
}