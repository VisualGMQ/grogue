#include "core/image.hpp"

namespace grogue::core  {

Image::Image(): texture_(nullptr) {}

Image::Image(Texture& texture)
    : texture_(&texture),
      srcRect_(0, 0, texture.GetSize().w, texture.GetSize().h),
      dstRect_(0, 0, texture.GetSize().w, texture.GetSize().h),
      anchor_(texture.GetSize().w / 2.0, texture.GetSize().h / 2.0) {
}

Image::Image(Texture& texture, const Recti& rect)
    : texture_(&texture),
      srcRect_(rect), dstRect_(0, 0, rect.w, rect.h),
      anchor_(srcRect_.w / 2.0, srcRect_.h / 2.0) {
}

Image Image::CreateImage(Texture& texture) {
    return Image(texture);
}

Image Image::CreateImage(Texture& texture, const Recti& rect) {
    return Image(texture, rect);
}

Image Image::CreateImage(std::string_view name) {
    auto texture = TextureMgr::Find(name.data());
    if (texture) {
        return CreateImage(*texture);
    } else {
        return Image();
    }
}

Image Image::CreateImage(std::string_view name, const Recti& rect) {
    auto texture = TextureMgr::Find(name.data());
    if (texture) {
        return CreateImage(*texture, rect);
    } else {
        return Image();
    }
}

}
