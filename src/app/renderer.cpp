#include "app/renderer.hpp"
#include "app/image.hpp"

Renderer::Renderer(Window& window) {
    renderer_ = SDL_CreateRenderer(window.window_, -1, 0);
    if (!renderer_) {
        Assert(renderer_ != nullptr, "renderer create failed");
    }
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer_);
}

Renderer::Renderer(Renderer&& renderer) {
    swap(*this, renderer);
}

Renderer& Renderer::operator=(Renderer&& o) {
    if (&o != this) {
        swap(*this, o);
        o.renderer_ = nullptr;
    }
    return *this;
}

void Renderer::SetDrawColor(const Color& color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

void Renderer::Present() {
    SDL_RenderPresent(renderer_);
}

void Renderer::Clear() {
    SDL_RenderClear(renderer_);
}

ImageHandle Renderer::LoadImage(const std::string& filename) {
    return ImageManager::Instance().Load(*this, filename);
}

void Renderer::DestroyImage(ImageHandle handle) {
    ImageManager::Instance().Destroy(handle);
}

void Renderer::DrawLine(const math::Vector2& p1, const math::Vector2& p2) {
    SDL_RenderDrawLineF(renderer_, p1.x, p1.y, p2.x, p2.y);
}

void Renderer::DrawRect(const math::Rect& rect) {
    SDL_FRect r = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderDrawRectF(renderer_, &r);
}

void Renderer::DrawImage(Image& image, const math::Rect& src, const math::Rect& des) {
    SDL_Rect srcRect = {static_cast<int>(src.x), static_cast<int>(src.y),
                        static_cast<int>(src.w), static_cast<int>(src.h)};
    SDL_FRect desRect = {des.x, des.y,
                         des.w, des.h};
    SDL_RenderCopyF(renderer_, image.texture_, &srcRect, &desRect);
}

Image* Renderer::GetImage(ImageHandle& handle) {
    if (ImageManager::Instance().Has(handle)) {
        return &ImageManager::Instance().Get(handle);
    } else {
        return nullptr;
    }
}