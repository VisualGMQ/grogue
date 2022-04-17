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

void Renderer::setColor(const Color& color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer_);
}

}
