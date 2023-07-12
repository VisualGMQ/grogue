#pragma once

#include "app/image.hpp"
#include "app/font.hpp"

class Renderer;

class [[refl, luabind]] TextTexture final {
public:
    TextTexture(Renderer* renderer, Font& font) : renderer_(renderer), font_(font) {}

    TextTexture(Renderer* renderer, const std::string& text, Font& font)
        : renderer_(renderer), font_(font) {
        SetText(text);
    }

    const std::unique_ptr<Image>& [[luanobind]] GetImage() const { return image_; }
    void SetText(const std::string& text);
    const std::string& GetText() const { return text_; }

private:
    Renderer* renderer_;    // due to Circular Reference, use Renderer* rather than Renderer&
    std::string text_;
    Font& font_;
    std::unique_ptr<Image> image_;
};

