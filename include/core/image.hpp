#pragma once

#include "texture.hpp"

namespace grogue::core  {

class Image final {
public:
    friend class Renderer;

    static Image CreateImage(Texture& texture);
    static Image CreateImage(Texture& texture, const Recti& rect);
    static Image CreateImage(std::string_view name);
    static Image CreateImage(std::string_view name, const Recti& rect);

    bool Valid() const { return texture_ != nullptr; }
    operator bool() const { return Valid(); }

    void SetPos(const Vec2& pos) { dstRect_.x = pos.x; dstRect_.y = pos.y; }
    void SetSize(const Vec2& size) { dstRect_.w = size.w; dstRect_.h = size.h; }
    Vec2 GetPos() const { return Vec2(dstRect_.x, dstRect_.y); }
    Vec2 GetSize() const { return Vec2(dstRect_.w, dstRect_.h); }
    Rect GetRect() const { return dstRect_; }
    float GetRotation() const { return degree_; }
    void SetRotation(float degree) { degree_ = degree; }
    void SetRotateAnchor(const Vec2& anchor) { anchor_ = anchor; }
    Vec2 GetRotateAnchor() const { return anchor_; }
    void SetFlip(Flip flip) { flip_ = flip; }
    Flip GetFlip() const { return flip_; }

private:
    Texture* texture_;
    Recti srcRect_;
    Rect dstRect_;
    float degree_ = 0;
    Vec2  anchor_;
    Flip flip_ = Flip::NoFlip;

    Image();
    Image(Texture& texture);
    Image(Texture& texture, const Recti& rect);
};

}
