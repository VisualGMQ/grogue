#include "core/pch.hpp"
#include "app/handle.hpp"
#include "app/renderer.hpp"
#include "app/renderer.hpp"
#include "app/fwd.hpp"
#include "app/manager.hpp"

class Image final {
public:
    friend class Renderer;
    friend class ImageManager;

    Image(const Image&) = delete;
    Image(Image&&);
    ~Image();

    Image& operator=(const Image&) = delete;
    Image& operator=(Image&&);

    int W() const { return w_; }
    int H() const { return h_; }

private:
    ImageHandle handle_;
    SDL_Texture* texture_ = nullptr;
    int w_ = 0;
    int h_ = 0;

    Image(ImageHandle handle, Renderer& renderer, const std::string& filename);

    friend void swap(Image& lhs, Image& rhs) {
        std::swap(lhs.handle_, rhs.handle_);
        std::swap(lhs.texture_, rhs.texture_);
        std::swap(lhs.w_, rhs.w_);
        std::swap(lhs.h_, rhs.h_);
    }
};

class ImageManager final : public Singleton<ImageManager, false>, public Manager<Image> {
public:
    ImageHandle Load(Renderer&, const std::string& filename);
};
