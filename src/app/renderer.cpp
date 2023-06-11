#include "app/renderer.hpp"
#include "app/image.hpp"
#include "app/renderer.hpp"

Renderer::Renderer(Window& window, FontManager& fontManager)
    : fontManager_(&fontManager) {
    renderer_ = SDL_CreateRenderer(window.window_, -1, 0);
    if (!renderer_) {
        Assert(renderer_ != nullptr, "renderer create failed");
    }
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BlendMode::SDL_BLENDMODE_BLEND);
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

Color Renderer::GetDrawColor() {
    Color color;
    SDL_GetRenderDrawColor(renderer_, &color.r, &color.g, &color.b, &color.a);
    return color;
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

void Renderer::FillRect(const math::Rect& rect) {
    SDL_FRect r = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderFillRectF(renderer_, &r);
}

void Renderer::DrawText(FontHandle handle, const std::string& text,
                        const Transform& transform) {
    if (fontManager_->Has(handle)) {
        DrawText(fontManager_->Get(handle), text.c_str(), transform);
    }
}

void Renderer::DrawText(Font& font, const std::string& text, const Transform& transform) {
    auto color = GetDrawColor();
    auto surface = TTF_RenderUTF8_Blended(
        font.font_, text.c_str(),
        {color.r, color.g, color.b, color.a});

    auto texture = SDL_CreateTextureFromSurface(renderer_, surface);
    drawTexture(
        texture, surface->w, surface->h,
        {0, 0, static_cast<float>(surface->w),
            static_cast<float>(surface->h)},
        {static_cast<float>(surface->w), static_cast<float>(surface->h)},
        transform, {0, 0}, Flip::None);
    SDL_FreeSurface(surface);
}

void Renderer::DrawCircle(const math::Vector2& center, float radius, float subsection) {
    static std::vector<SDL_FPoint> points;
    float stepAngle = 360.0 / subsection;
    points.resize(subsection);
    float radians = math::Deg2Rad(stepAngle);

    for (int i = 0; i < points.size(); i++) {
        points[i].x = center.x + radius * std::cos(i * radians);
        points[i].y = center.y + radius * std::sin(i * radians);
    }

    SDL_RenderDrawLinesF(renderer_, points.data(), points.size());
    SDL_RenderDrawLineF(renderer_, points[0].x, points[0].y, points.back().x, points.back().y);
}

void Renderer::DrawSprite(const SpriteBundle& sprite, const Transform& transform) {
    if (!sprite.visiable) return;

    auto& image = imageManager_->Get(sprite.image);
    SDL_SetTextureColorMod(image.texture_, sprite.sprite.color.r, sprite.sprite.color.g, sprite.sprite.color.b);
    SDL_SetTextureAlphaMod(image.texture_, sprite.sprite.color.a);
    drawTexture(image.texture_, image.W(), image.H(), sprite.sprite.region,
                sprite.sprite.customSize, transform,
                sprite.sprite.anchor, sprite.flip);
}

void Renderer::DrawSprite(const SpriteBundleSolitary& sprite, const Transform& transform) {
    if (!sprite.visiable) return;

    auto& image = sprite.image;
    SDL_SetTextureColorMod(image.texture_, sprite.sprite.color.r, sprite.sprite.color.g, sprite.sprite.color.b);
    SDL_SetTextureAlphaMod(image.texture_, sprite.sprite.color.a);
    drawTexture(image.texture_, image.W(), image.H(), sprite.sprite.region,
                sprite.sprite.customSize, transform,
                sprite.sprite.anchor, sprite.flip);
}

void Renderer::DrawShape(const Shape& shape, const Transform& transform) {
    static std::vector<float> uvCache;

    Assert(!shape.vertices.empty(), "shape position or colors must > 0");

    int result;
    // don't need transform, we use raw datas
    if (transform.position == math::Vector2::Zero && transform.rotation == 0 && transform.scale == math::Vector2(1, 1)) {
        result = SDL_RenderGeometry(renderer_, nullptr,
                            (SDL_Vertex*)shape.vertices.data(), shape.vertices.size(),
                            shape.indices.has_value() ? (const int*)shape.indices.value().data() : nullptr,
                            shape.indices.has_value() ? shape.indices.value().size() : 0);
    } else {    // need transform, we must copy data on heap and change them
        auto vertices = shape.vertices;
        for (auto& vertex : vertices) {
            vertex.position = math::Translate(math::Rotate(math::Scale(vertex.position, transform.scale), math::Deg2Rad(transform.rotation)), transform.position);
        }

        result = SDL_RenderGeometry(renderer_, nullptr,
                            (SDL_Vertex*)vertices.data(), vertices.size(),
                            shape.indices.has_value() ? (const int*)shape.indices.value().data() : nullptr,
                            shape.indices.has_value() ? shape.indices.value().size() : 0);
    }
    if (result == -1) {
        LOGW("SDL_RenderGeometryRaw error: ", SDL_GetError());
    }
}

std::shared_ptr<TextTexture> Renderer::GenTextTexture(const std::string& text, Font& font) {
    return std::make_shared<TextTexture>(this, text, font);
}

std::unique_ptr<Image> Renderer::GenTextImage(const std::string& text, Font& font) {
    auto surface = TTF_RenderUTF8_Blended(
        font.font_, text.c_str(),
        {255, 255, 255, 255});

    auto texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);

    return std::unique_ptr<Image>(new Image(ImageHandle::Null(), texture));
}

void Renderer::SetClipArea(const math::Rect& area) {
    SDL_Rect rect;
    rect.x = area.x;
    rect.y = area.y;
    rect.w = area.w;
    rect.h = area.h;
    if (SDL_RenderSetClipRect(renderer_, &rect) != 0) {
        LOGE("set render clip rect failed: ", SDL_GetError());
    }
}

void Renderer::SetDefaultClipArea() {
    SDL_RenderSetClipRect(renderer_, nullptr);
}

void Renderer::drawTexture(SDL_Texture* texture, int rawW, int rawH,
                           const math::Rect& region, const math::Vector2& size,
                           const Transform& transform, const math::Vector2& anchor,
                           Flip flip) {
    if (!texture) return;

    SDL_Rect srcRect;
    srcRect.x = region.x;
    srcRect.y = region.y;
    srcRect.w = region.w <= 0 ? rawW : region.w;
    srcRect.h = region.h <= 0 ? rawH : region.h;

    math::Vector2 finalSize = {size.x <= 0 ? rawW : size.x,
                               size.y <= 0 ? rawH : size.y};
    finalSize.x *= transform.scale.x;
    finalSize.y *= transform.scale.y;

    SDL_FRect desRect;
    desRect.x = transform.position.x - finalSize.x * anchor.x;
    desRect.y = transform.position.y - finalSize.y * anchor.y;
    desRect.w = finalSize.x;
    desRect.h = finalSize.y;

    SDL_FPoint rotAnchor;
    rotAnchor.x = anchor.x * finalSize.x;
    rotAnchor.y = anchor.y * finalSize.y;

    SDL_RenderCopyExF(renderer_, texture, &srcRect, &desRect,
                      transform.rotation, &rotAnchor,
                      static_cast<SDL_RendererFlip>(flip));
}
