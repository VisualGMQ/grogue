#pragma once
#include "pch.hpp"
#include "window.hpp"
#include "mathf.hpp"
#include "texture.hpp"
#include "storage.hpp"
#include "font.hpp"
#include "image.hpp"

namespace grogue::core {

struct Transform {
    Transform();

    Transform& SetSrcArea(const Recti& r);
    Transform& SetPos(const Vec2& v);
    Transform& SetScale(const Vec2& s);
    Transform& SetFlip(Flip);
    Transform& SetRotation(float);
    Transform& SetRotatAnchor(const Vec2& a);

    Vec2 position;
    Vec2 scale;
    Flip flip = Flip::NoFlip;
    float degree = 0;
    Vec2 anchor;
};



class Renderer final {
public:
    friend class TextureManager;
    friend class Texture;
    friend class Engine;

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
                  const std::optional<Color>& border = std::nullopt);

    std::unique_ptr<Texture> GenerateText(Font& font, const char* text, const Color&);

    void DrawImage(const Image&, const Transform&);

    bool IsClipping() const;
    Recti GetClipRect() const;
    void SetClip(const Recti& r);

    /* for imgui, don't use in other places */
    SDL_Renderer* GetRawRenderer() const { return renderer_; }

private:
    SDL_Renderer* renderer_;

    void setColor(const Color&);
};


}
