#pragma once

#include "SDL.h"
#include "app/window.hpp"
#include "core/assert.hpp"
#include "core/math.hpp"
#include "app/fwd.hpp"

struct Color {
    int r, g, b, a;

    Color(int r, int g, int b, int a = 255) : r(r), g(g), b(b), a(a) {}
};

class Renderer final {
public:
    friend class Image;

    Renderer(Window& window);
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&);
    ~Renderer();

    ImageHandle LoadImage(const std::string& filename);
    void DestroyImage(ImageHandle);

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&);

    void SetDrawColor(const Color&);

    void DrawLine(const math::Vector2&, const math::Vector2&);
    void DrawRect(const math::Rect&);
    void DrawImage(Image&, const math::Rect& src, const math::Rect& des);
    Image* GetImage(ImageHandle&);

    void Present();
    void Clear();

private:
    SDL_Renderer* renderer_ = nullptr;

    // use Copy-And-Swap-Idiom:
    // https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    friend void swap(Renderer& lhs, Renderer& rhs) noexcept {
        std::swap(lhs.renderer_, rhs.renderer_);
    }
};