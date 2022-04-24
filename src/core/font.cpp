#include "core/font.hpp"

namespace grogue::core {

Font::Font(const char* filename, int pt) {
    font_ = TTF_OpenFont(filename, pt);
    if (!font_) {
        LOG_ERROR("font %s load failed",  filename);
    }
}

Font::~Font() {
    TTF_CloseFont(font_);
}

Size Font::GetSize(const char* s) {
    int w, h;
    TTF_SizeUTF8(font_, s, &w, &h);
    return {w, h};
}

Size Font::GetSize(const std::string& s) {
    int w, h;
    TTF_SizeUTF8(font_, s.c_str(), &w, &h);
    return {w, h};
}


Storage<std::string, std::unique_ptr<Font>> FontMgr::fonts_;

Font* FontMgr::Create(const std::string& filename, int pt) {
    return Create(filename, filename, pt);
}

Font* FontMgr::Create(const std::string& filename,
                    const std::string& name, int pt) {
    if (const auto& font = fonts_.Find(name)) {
        return font->get();
    } else {
        return fonts_.Create(name, std::make_unique<Font>(filename.c_str(), pt)).get();
    }
}

Font* FontMgr::Find(const std::string& name) {
    if (const auto& font = fonts_.Find(name)) {
        return font->get();
    } else {
        return nullptr;
    }
}

void FontMgr::Clear() {
    fonts_.Clear();
}

}
