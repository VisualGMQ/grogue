#include "core/image.hpp"

namespace grogue::core  {

Image::Image(): texture_(nullptr) {}

Image::Image(Texture& texture)
    : texture_(&texture),
      srcRect_(0, 0, texture.GetSize().w, texture.GetSize().h) {}

Image::Image(Texture& texture, const Recti& rect)
    : texture_(&texture),
      srcRect_(rect) {}

Image Image::Create(Texture& texture) {
    return Image(texture);
}

Image Image::Create(Texture& texture, const Recti& rect) {
    return Image(texture, rect);
}

Image Image::Create(std::string_view name) {
    auto texture = TextureMgr::Find(name.data());
    if (texture) {
        return Create(*texture);
    } else {
        return Image();
    }
}

Image Image::Create(std::string_view name, const Recti& rect) {
    auto texture = TextureMgr::Find(name.data());
    if (texture) {
        return Create(*texture, rect);
    } else {
        return Image();
    }
}

Image Image::Create(const Image& image, const Recti& rect) {
    if (image.texture_) {
        return Create(*image.texture_,
                      Recti(image.srcRect_.x + rect.x, image.srcRect_.y + rect.y,
                            rect.w, rect.h));
    } else {
        return Image();
    }
}

}
