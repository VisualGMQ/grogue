#include "core/renderer.hpp"
#include "core/texture.hpp"
#include "core/video.hpp"

namespace grogue::core {

Renderer::Renderer(Window& window) {
    renderer_ = SDL_CreateRenderer(window.window_, -1, 0);
}

void Renderer::Present() {
    SDL_RenderPresent(renderer_);
}

void Renderer::Clear(const Color& color) {
    setColor(color);
    SDL_RenderClear(renderer_);
}

void Renderer::DrawLine(const Vector<float, 2>& p1, const Vector<float, 2>& p2,
                        const Color& color) {
    setColor(color);
    SDL_RenderDrawLineF(renderer_, p1.x, p1.y, p2.x, p2.y);
}

void Renderer::DrawRect(const Rect& rect, const Color& color) {
    setColor(color);
    SDL_RenderDrawRectF(renderer_, &rect.sdlrect);
}

void Renderer::FillRect(const Rect& rect, const Color& color) {
    setColor(color);
    SDL_RenderFillRectF(renderer_, &rect.sdlrect);
}

void Renderer::DrawBorderRect(const Rect& rect, const Color& border, const Color& fill) {
    FillRect(rect, fill);
    DrawRect(rect, border);
}

void Renderer::DrawTexture(const TextureRenderInfo& info) {
    SDL_FPoint center = {info.anchor.x, info.anchor.y};
    SDL_RenderCopyExF(renderer_,
                      info.texture->texture_,
                      &info.src.sdlrect,
                      &info.dst.sdlrect,
                      info.degree,
                      &center,
                      static_cast<SDL_RendererFlip>(info.flip));
}

void Renderer::setColor(const Color& color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer_);
}

Renderer::TextureRenderInfo::TextureRenderInfo(Texture* texture): texture(texture) {
    dst.x = dst.y = 0;
    dst.w = texture->GetSize().w;
    dst.h = texture->GetSize().h;
    src.x = src.y = 0;
    src.w = dst.w;
    src.h = dst.h;
    anchor.x = src.w / 2.0;
    anchor.y = src.h / 2.0;
}

Renderer::TextureRenderInfo& Renderer::TextureRenderInfo::SetSrcArea(const Recti& r) {
    src = r;
    return *this;
}

Renderer::TextureRenderInfo& Renderer::TextureRenderInfo::SetPos(const Vec2& v) {
    dst.x = v.x;
    dst.y = v.y;
    return *this;
}


Renderer::TextureRenderInfo& Renderer::TextureRenderInfo::SetSize(const Vec2& s) {
    dst.w = s.w;
    dst.h = s.h;
    return *this;
}

Renderer::TextureRenderInfo& Renderer::TextureRenderInfo::SetFlip(Flip f) {
    flip = f;
    return *this;
}

Renderer::TextureRenderInfo& Renderer::TextureRenderInfo::SetRotation(float degree) {
    this->degree = degree;
    return *this;
}

Renderer::TextureRenderInfo& Renderer::TextureRenderInfo::SetRotatAnchor(const Vec2& a) {
    anchor = a;
    return *this;
}


}
