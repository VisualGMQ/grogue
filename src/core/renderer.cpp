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

void Renderer::DrawRect(const Rect& rect,
                        const std::optional<Color>& fill,
                        const std::optional<Color>& border) {
    if (fill) {
        setColor(fill.value());
        SDL_RenderFillRectF(renderer_, &rect.sdlrect);
    }
    if (border) {
        setColor(border.value());
        SDL_RenderDrawRectF(renderer_, &rect.sdlrect);
    }
}

void Renderer::setColor(const Color& color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer_);
}

Transform::Transform()
    :position(0, 0), scale(1, 1), flip(Flip::NoFlip), degree(0), anchor(0.5, 0.5) {}

Transform& Transform::SetPos(const Vec2& v) {
    position = v;
    return *this;
}


Transform& Transform::SetScale(const Vec2& s) {
    scale = s;
    return *this;
}

Transform& Transform::SetFlip(Flip f) {
    flip = f;
    return *this;
}

Transform& Transform::SetRotation(float degree) {
    this->degree = degree;
    return *this;
}

Transform& Transform::SetRotatAnchor(const Vec2& a) {
    anchor = a;
    return *this;
}

std::unique_ptr<Texture> Renderer::GenerateText(Font& font, const char* text, const Color& color) {
    auto surface = TTF_RenderUTF8_Blended(font.font_, text, color.color);
    auto texture = std::make_unique<Texture>(surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Renderer::DrawImage(const Image& image, const Transform& transform) {
    if (!image) return;
    SDL_FPoint center = {image.srcRect_.x + image.srcRect_.w * transform.anchor.x,
                         image.srcRect_.y + image.srcRect_.h * transform.anchor.y};
    SDL_FRect dstRect{transform.position.x, transform.position.y,
                      transform.scale.x * image.srcRect_.w,
                      transform.scale.y * image.srcRect_.h};
    SDL_RenderCopyExF(renderer_,
                      image.texture_->texture_,
                      &image.srcRect_.sdlrect,
                      &dstRect,
                      transform.degree,
                      &center,
                      static_cast<SDL_RendererFlip>(transform.flip));
}

bool Renderer::IsClipping() const {
    return SDL_RenderIsClipEnabled(renderer_);
}

void Renderer::SetClip(const Recti& r) {
    SDL_RenderSetClipRect(renderer_, &r.sdlrect);
}

Recti Renderer::GetClipRect() const {
    Recti rect;
    SDL_RenderGetClipRect(renderer_, &rect.sdlrect);
    return rect;
}

}
