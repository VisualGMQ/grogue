#include "engine/renderer/font.hpp"

namespace engine {

Font::Font(FontID id, const std::string& filename, const std::string& name, int pt)
    : pt_(pt), name_(name) {
    font_ = TTF_OpenFont(filename.c_str(), pt);
    if (!font_) {
        Loge("load font {} failed", filename);
    }
}

Font::~Font() {
    TTF_CloseFont(font_);
}

Vec2 Font::SizeUTF8(const std::string& text) const {
    int w, h;
    TTF_SizeUTF8(font_, text.c_str(), &w, &h);
    return Vec2(w, h);
}


std::unordered_map<FontID, std::unique_ptr<Font>> FontFactory::fontMap_;
FontID FontFactory::curId_ = 0;

void FontFactory::Init() {}
void FontFactory::Quit() {
    fontMap_.clear();
}

Font* FontFactory::Find(FontID id) {
    auto it = fontMap_.find(id);
    return it == fontMap_.end() ? nullptr : it->second.get();
}

Font* FontFactory::Find(const std::string& name) {
    for (auto& [key, font] : fontMap_) {
        if (font->Name() == name) {
            return font.get();
        }
    }
    return nullptr;
}

Font* FontFactory::Create(const std::string& filename, const std::string& name, int pt) {
    Font* font = Find(name);
    if (font) {
        Loge("font {} already exists", name);
        return font;
    }
    FontID id = curId_++;
    auto f = std::make_unique<Font>(id, filename, name, pt);
    font = f.get(); 
    fontMap_[id] = std::move(f);
    return font;
}

}