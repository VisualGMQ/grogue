#include "app/renderer.hpp"
#include "app/image.hpp"

Renderer::Renderer(Window& window, FontManager& fontManager): fontManager_(&fontManager) {
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

void Renderer::DrawLine(const math::Vector2& p1, const math::Vector2& p2) {
    SDL_RenderDrawLineF(renderer_, p1.x, p1.y, p2.x, p2.y);
}

void Renderer::DrawRect(const math::Rect& rect) {
    SDL_FRect r = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderDrawRectF(renderer_, &r);
}

void Renderer::DrawImage(ImageHandle& handle, const math::Rect& src, const Transform& transform) {
    if (!handle) {
        return ;
    }
    auto& image = imageManager_->Get(handle);
    SDL_Rect srcRect;
    srcRect.x = src.x;
    srcRect.y = src.y;
    srcRect.w = src.w <= 0 ? image.W() : src.w;
    srcRect.h = src.h <= 0 ? image.H() : src.h;

    SDL_FRect desRect;
    desRect.x = transform.GetPos().x;
    desRect.y = transform.GetPos().y;
    desRect.w = transform.GetSize().x <= 0 ? image.W() * std::abs(transform.GetSize().x): transform.GetSize().x;
    desRect.h = transform.GetSize().y <= 0 ? image.H() * std::abs(transform.GetSize().y): transform.GetSize().y;

    auto anchor = transform.GetRotateAnchor() * transform.GetSize();
    SDL_FPoint rotAnchor;
    rotAnchor.x = anchor.x;
    rotAnchor.y = anchor.y;

    SDL_RenderCopyExF(renderer_, image.texture_, &srcRect, &desRect,
                      transform.GetRotation(), &rotAnchor,
                      static_cast<SDL_RendererFlip>(transform.GetFlip()));
}

void Renderer::DrawText(FontHandle handle, const std::string& text,
                        const math::Vector2& pos, const Color& color) {
    if (fontManager_->Has(handle)) {
        auto surface = TTF_RenderUTF8_Blended(
            fontManager_->Get(handle).font_, text.c_str(),
            {color.r, color.g, color.b, color.a});
        auto texture = SDL_CreateTextureFromSurface(renderer_, surface);
        SDL_Rect dst{static_cast<int>(pos.x), static_cast<int>(pos.y),
                     surface->w, surface->h};
        SDL_RenderCopy(renderer_, texture, nullptr, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}