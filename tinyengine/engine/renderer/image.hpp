#pragma once

#include <functional>

namespace engine {

class Texture;

struct Image final {
    Texture* texture;
    Rect region;

    Image(): texture(nullptr) {} // FIXME use Empty Object to replace texture
    Image(Texture* texture, const Rect& region): texture(texture), region(region) {}
    Image(Texture* texture);

    operator bool() const {
        return texture && region.size.w > 0 && region.size.h > 0;
    }
};

class ImageFactory final {
public:
    static Image Create(Texture* texture, const std::string& name, const std::optional<Rect>& region);
    static bool Find(const std::string& name, Image& outImage);

private:
    static std::unordered_map<std::string, Image> images_;
};

}
