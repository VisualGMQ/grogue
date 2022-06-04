#pragma once
#include "renderer.hpp"

namespace grogue::core {

class GFX final {
public:
    static void Init(Renderer* renderer);
    static void Shutdown();

    static void Clear(const Color&);
    static void Present();

    static void DrawLine(const Vector<float, 2>& p1, const Vector<float, 2>& p2,
                  const Color&);
    static void DrawRect(const Rect& rect,
                  const std::optional<Color>& fill,
                  const std::optional<Color>& border = std::nullopt);

    static std::unique_ptr<Texture> GenerateText(Font& font, const char* text, const Color&);

    static void DrawImage(const Image&, const Transform&);

    static bool IsClipping();
    static Recti GetClipRect();
    static void SetClip(const Recti& r);

private:
    static Renderer* renderer_;
};

}
