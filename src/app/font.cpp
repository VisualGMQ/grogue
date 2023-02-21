#include "app/font.hpp"

Font::Font(Font&& f): handle_(FontHandle::Null()) {
    swap(*this, f);
}

Font& Font::operator=(Font&& f) {
    if (this != &f) {
        swap(*this, f);
        f.font_ = nullptr;
        f.handle_ = FontHandle::Null();
    }

    return *this;
}

Font::Font(FontHandle handle, const std::string& filename, int ptsize): handle_(handle) {
    font_ = TTF_OpenFont(filename.c_str(), ptsize);

    if (!font_) {
        LOGE("load font ", filename, " failed: ", TTF_GetError());
        FontHandle::Destroy(handle_);
    }
}

Font::~Font() {
    FontHandle::Destroy(handle_);
    TTF_CloseFont(font_);
}


FontHandle FontManager::Load(const std::string& filename, int ptsize) {
    auto newHandle = FontHandle::Create();
    storeNewItem(newHandle, std::unique_ptr<Font>(new Font(newHandle, filename, ptsize)));
    return newHandle;
}
