#pragma once
#include "pch.hpp"
#include "window.hpp"
#include "mathf.hpp"

namespace grogue::core {

class Renderer final {
public:
    friend class TextureManager;

    Renderer(Window& window);
    Renderer(const Renderer&) = delete;
    ~Renderer();

    Renderer& operator=(const Renderer&) = delete;

    void Clear(const Color&);
    void Present();

    void DrawLine(const Vector<float, 2>& p1, const Vector<float, 2>& p2, const Color&);
    void DrawRect(const Rect& rect, const Color&);
    void FillRect(const Rect& rect, const Color&);
    void DrawBorderRect(const Rect& rect, const Color& border, const Color& fill);

    void DrawTexture(/* TODO not finish */);
    void DrawFont(/* TODO not finish */);
    void SetTarget(/* TODO not finish */);

private:
    SDL_Renderer* renderer_;

    void setColor(const Color&);
};


}
