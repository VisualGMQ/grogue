#include "app/text_texture.hpp"
#include "app/renderer.hpp"

void TextTexture::SetText(const std::string& text) {
    text_ = text;
    image_ = renderer_->GenTextImage(text_, font_);
}