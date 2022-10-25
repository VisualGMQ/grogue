#include "engine/renderer/renderer.hpp"

namespace engine {

Color Renderer::clearColor_{0, 0, 0};

void Renderer::Init(int w, int h) {
    if (TTF_Init() != 0) {
        Loge("SDL ttf init failed: {}", TTF_GetError());
    }
    if (IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF) == 0) {
        Loge("SDL image init failed: {}", IMG_GetError());
    }
}

void Renderer::Quit() {
    IMG_Quit();
    TTF_Quit();
}

void Renderer::SetClearColor(const Color& color) {
    clearColor_ = color;
}

void Renderer::Clear() {
    SDL_RenderClear(Video::GetRenderer());
}

void Renderer::SetDrawColor(const Color& color) {
    SDL_SetRenderDrawColor(Video::GetRenderer(), color.r, color.g, color.b, color.a);
}

void Renderer::DrawRect(const Rect& rect) {
    SDL_RenderDrawRectF(Video::GetRenderer(), &rect.sdl_rect);
}

void Renderer::DrawLine(const Vec2& p1, const Vec2& p2) {
    SDL_RenderDrawLineF(Video::GetRenderer(), p1.x, p1.y, p2.x, p2.y);
}

void Renderer::DrawLines(const std::vector<Vec2>& points) {
    std::vector<SDL_FPoint> pts(points.size());
    for (auto& pt : points) {
        pts.push_back(SDL_FPoint{pt.x, pt.y});
    }
    SDL_RenderDrawLinesF(Video::GetRenderer(), pts.data(), pts.size());
}

void Renderer::DrawLineLoop(const std::vector<Vec2>& points) {
    std::vector<SDL_FPoint> pts(points.size());
    for (auto& pt : points) {
        pts.push_back(SDL_FPoint{pt.x, pt.y});
    }
    SDL_RenderDrawLinesF(Video::GetRenderer(), pts.data(), pts.size());
    SDL_RenderDrawLine(Video::GetRenderer(), pts.back().x, pts.back().y, pts.front().x, pts.front().y);
}

void Renderer::FillRect(const Rect& rect) {
    SDL_RenderFillRectF(Video::GetRenderer(), &rect.sdl_rect);
}

void Renderer::DrawTexture(const Texture& texture, const Rect* const src, const Size& size, const Transform& transform) {
    Rect dstRect;
    dstRect.position = transform.position;
    dstRect.size = size * transform.scale;
    dstRect.size.w = std::abs(dstRect.size.w);
    dstRect.size.h = std::abs(dstRect.size.h);

    std::optional<SDL_Rect> srcRect;
    if (src) {
        srcRect = SDL_Rect{(int)src->position.x, (int)src->position.y, (int)src->size.w, (int)src->size.h};
    }
    unsigned int flip = SDL_FLIP_NONE;
    if (transform.scale.x < 0) {
        flip |= SDL_FLIP_HORIZONTAL;
    }
    if (transform.scale.y < 0) {
        flip |= SDL_FLIP_VERTICAL;
    }
    SDL_RendererFlip rflip = static_cast<SDL_RendererFlip>(flip);
    SDL_RenderCopyExF(Video::GetRenderer(), texture.texture_,
                      srcRect ? &srcRect.value() : nullptr, &dstRect.sdl_rect,
                      transform.rotation, &transform.anchor.sdl_point,
                      static_cast<SDL_RendererFlip>(flip));
}

void Renderer::DrawText(Font* font, const std::string& text, const Vec2& pos) {
    if (!font) return;
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font->font_, text.c_str(), SDL_Color{255, 255, 255, 255});

    Texture texture(surface);

    Transform transform;
    transform.position = pos;
    DrawTexture(texture,
                nullptr,
                Size(surface->w, surface->h),
                transform);

    SDL_FreeSurface(surface);
}

}
