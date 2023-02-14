#pragma once

#include "app/font.hpp"
#include "app/fwd.hpp"
#include "app/window.hpp"
#include "core/math.hpp"
#include "core/pch.hpp"
#include "app/transform.hpp"
#include "app/sprite.hpp"

class Image;
class ImageManager;

class Renderer final {
public:
    friend class Image;
    friend class DefaultPlugins;

    Renderer(Window& window, FontManager& fontManager);
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&);
    ~Renderer();

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&);

    void SetDrawColor(const Color&);
    Color GetDrawColor();

    void DrawLine(const math::Vector2&, const math::Vector2&);
    void DrawRect(const math::Rect&);
    void DrawText(FontHandle, const std::string& text, const Transform&);
    void DrawSprite(SpriteBundle&);

    void Present();
    void Clear();

    SDL_Renderer* Raw() const { return renderer_; }

private:
    SDL_Renderer* renderer_ = nullptr;
    FontManager* fontManager_ = nullptr;
    ImageManager* imageManager_ = nullptr;

    // use Copy-And-Swap-Idiom:
    // https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    friend void swap(Renderer& lhs, Renderer& rhs) noexcept {
        std::swap(lhs.renderer_, rhs.renderer_);
        std::swap(lhs.fontManager_, rhs.fontManager_);
        std::swap(lhs.imageManager_, rhs.imageManager_);
    }

    void drawTexture(SDL_Texture* texture, int rawW, int rawH,
                     const math::Rect& region, const math::Vector2& size,
                     const Transform& transform, const math::Vector2& anchor,
                     Flip flip);
};