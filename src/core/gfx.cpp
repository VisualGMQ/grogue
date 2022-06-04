#include "core/gfx.hpp"

namespace grogue::core {

Renderer* GFX::renderer_ = nullptr;

void GFX::Init(Renderer* renderer) {
    renderer_ = renderer;
}

void GFX::Shutdown() {}

void GFX::Clear(const Color& color) {
    renderer_->Clear(color);
}

void GFX::Present() {
    renderer_->Present();
}

void GFX::DrawLine(const Vector<float, 2>& p1, const Vector<float, 2>& p2,
                   const Color& color) {
    renderer_->DrawLine(p1, p2, color);
}

void GFX::DrawRect(const Rect& rect,
       const std::optional<Color>& fill,
       const std::optional<Color>& border) {
    renderer_->DrawRect(rect, fill, border);
}

std::unique_ptr<Texture> GFX::GenerateText(Font& font,
                                           const char* text,
                                           const Color& color) {
    return renderer_->GenerateText(font, text, color);
}

void GFX::DrawImage(const Image& img, const Transform& transform) {
    renderer_->DrawImage(img, transform);
}

bool GFX::IsClipping() {
    return renderer_->IsClipping();
}

Recti GFX::GetClipRect() {
    return renderer_->GetClipRect();
}

void GFX::SetClip(const Recti& r) {
    renderer_->SetClip(r);
}



}
