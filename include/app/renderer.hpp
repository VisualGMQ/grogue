#pragma once

#include "app/font.hpp"
#include "app/fwd.hpp"
#include "app/window.hpp"
#include "core/math.hpp"
#include "core/pch.hpp"
#include "app/transform.hpp"

class Image;
class ImageManager;

struct Color {
    unsigned char r, g, b, a;

    Color(unsigned char r, unsigned char g, unsigned char b,
          unsigned char a = 255)
        : r(r), g(g), b(b), a(a) {}
};

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

    void DrawLine(const math::Vector2&, const math::Vector2&);
    void DrawRect(const math::Rect&);
    void DrawImage(ImageHandle&, const math::Rect& src, const Transform&);
    void DrawText(FontHandle, const std::string& text, const math::Vector2& pos,
                  const Color&);

    void Present();
    void Clear();

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
};