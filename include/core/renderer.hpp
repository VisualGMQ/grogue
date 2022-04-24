#pragma once
#include "pch.hpp"
#include "window.hpp"
#include "mathf.hpp"
#include "texture.hpp"
#include "storage.hpp"
#include "font.hpp"

namespace grogue::core {

class Renderer final {
public:
    friend class TextureManager;
    friend class Texture;

    Renderer(Window& window);
    Renderer(const Renderer&) = delete;
    ~Renderer();

    Renderer& operator=(const Renderer&) = delete;

    void Clear(const Color&);
    void Present();

    void DrawLine(const Vector<float, 2>& p1, const Vector<float, 2>& p2,
                  const Color&);
    void DrawRect(const Rect& rect,
                  const std::optional<Color>& fill,
                  const std::optional<Color>& border);

    struct TextureRenderInfo {
    public:
        friend class Renderer;

        TextureRenderInfo(Texture* texture);
        TextureRenderInfo& SetSrcArea(const Recti& r);
        TextureRenderInfo& SetPos(const Vec2& v);
        TextureRenderInfo& SetSize(const Vec2& s);
        TextureRenderInfo& SetFlip(Flip);
        TextureRenderInfo& SetRotation(float);
        TextureRenderInfo& SetRotatAnchor(const Vec2& a);

    private:
        Texture* texture;
        Recti src;
        Rect dst;
        Flip flip = Flip::NoFlip;
        float degree = 0;
        Vec2 anchor;
    };

    void DrawTexture(const TextureRenderInfo& info);
    std::unique_ptr<Texture> GenerateText(Font& font, const char* text, const Color&);

private:
    SDL_Renderer* renderer_;

    void setColor(const Color&);
};


}
