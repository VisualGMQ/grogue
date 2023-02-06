#pragma once

#include "SDL.h"
#include "app/window.hpp"
#include "core/assert.hpp"
#include "app/image.hpp"
#include "core/math.hpp"

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

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&);

    void SetDrawColor(const Color&);
    void Present();
    void Clear();

private:
    SDL_Renderer* renderer_;
};