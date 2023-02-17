#pragma once
 
#include "core/pch.hpp"
#include "app/handle.hpp"
#include "app/fwd.hpp"
#include "app/manager.hpp"
#include "app/renderer.hpp"

class Image final {
public:
    friend class Renderer;
    friend class ImageManager;

    Image(const Image&) = delete;
    Image(Image&&);
    ~Image();

    Image& operator=(const Image&) = delete;
    Image& operator=(Image&&);

    int W() const { return w_; }
    int H() const { return h_; }
    math::Vector2 Size() const { return math::Vector2(w_, h_); }

private:
    ImageHandle handle_;
    SDL_Texture* texture_ = nullptr;
    int w_ = 0;
    int h_ = 0;

    Image(ImageHandle handle, Renderer& renderer, const std::string& filename);
    Image(ImageHandle handle, SDL_Texture*);

    friend void swap(Image& lhs, Image& rhs) {
        std::swap(lhs.handle_, rhs.handle_);
        std::swap(lhs.texture_, rhs.texture_);
        std::swap(lhs.w_, rhs.w_);
        std::swap(lhs.h_, rhs.h_);
    }
};

class ImageManager final : public ResourceManager<Image> {
public:
    ImageManager(Renderer&);
    ImageHandle Load(const std::string& filename);
    std::unique_ptr<Image> CreateSolitary(SDL_Texture*);

private:
    Renderer* renderer_;
};
