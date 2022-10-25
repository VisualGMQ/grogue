#include "engine/renderer/image.hpp"
#include "engine/renderer/texture.hpp"
    
namespace engine {

Image::Image(Texture* texture): texture(texture) {
    if (texture) {
        region.position.Set(0, 0);
        region.size = texture->GetSize();
    }
}

std::unordered_map<std::string, Image> ImageFactory::images_;

Image ImageFactory::Create(Texture* texture, const std::string& name, const std::optional<Rect>& region) {
    Image result;
    if (Find(name, result)) {
        return result;
    }
    if (region) {
        Image image(texture, region.value());
        return images_.emplace(name, std::move(image)).first->second;
    } else {
        Image image(texture);
        return images_.emplace(name, std::move(image)).first->second;
    }
}

bool ImageFactory::Find(const std::string& name, Image& outImage) {
    for (auto& [key, image] : images_) {
        if (key == name) {
            outImage = image;
            return true;
        }
    }
    return false;
}

}