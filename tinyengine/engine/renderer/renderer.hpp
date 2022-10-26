#pragma once

#include "engine/core/pch.hpp"
#include "engine/core/vmath.hpp"
#include "engine/core/tool.hpp"
#include "engine/renderer/texture.hpp"
#include "engine/renderer/font.hpp"
#include "engine/core/video.hpp"

namespace engine {

struct Transform {
    Vec2 position;
    Vec2 scale{1, 1};
    float rotation = 0; /* in radians */
    Vec2 anchor;
};

class Renderer {
public:
    Renderer() = delete;

    static void Init(int w, int h);
    static void Quit();

    static void SetClearColor(const Color&);
    static void Clear();
    static void SetDrawColor(const Color&);

    static void DrawRect(const Rect&);
    static void DrawLine(const Vec2&, const Vec2&);
    static void DrawLines(const std::vector<Vec2>&);
    static void DrawLineLoop(const std::vector<Vec2>&);
    static void FillRect(const Rect& rect);
    static void DrawTexture(const Texture& texture, const Rect* const src, const Size& size, const Transform&);
    static void DrawText(Font* font, const std::string& text, const Vec2& pos, const engine::Color&);

private:
    static Color clearColor_;
};

}
