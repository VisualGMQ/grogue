#pragma once

#include "texture.hpp"
#include "mathf.hpp"

namespace grogue::core  {

class Image final {
public:
    friend class Renderer;

    static Image Create(Texture& texture);
    static Image Create(Texture& texture, const Recti& rect);
    static Image Create(std::string_view name);
    static Image Create(std::string_view name, const Recti& rect);
    static Image Create(const Image& image, const Recti& rect);

    bool Valid() const { return texture_ != nullptr; }
    operator bool() const { return Valid(); }
    Size GetSize() const { return Size(srcRect_.w, srcRect_.h); }

private:
    Texture* texture_;
    Recti srcRect_;

    Image();
    Image(Texture& texture);
    Image(Texture& texture, const Recti& rect);
};

}
