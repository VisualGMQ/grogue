#include "core/renderer.hpp"

namespace grogue::core {

Renderer::Renderer(Window& window) {
    renderer_ = SDL_CreateRenderer(window.window_, -1, 0);
}

void Renderer::SetDrawColor(const Color& c) {
    SDL_SetRenderDrawColor(renderer_, c.r, c.g, c.b, c.a);
}

void Renderer::Present() {
    SDL_RenderPresent(renderer_);
}

void Renderer::Clear() {
    SDL_RenderClear(renderer_);
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(renderer_);
}

}
