#include "SDL.h"
#include "SDL_image.h"
#include "app/handle.hpp"
#include "app/renderer.hpp"
#include "core/assert.hpp"
#include "app/renderer.hpp"
#include "app/fwd.hpp"
#include "core/singlton.hpp"
#include "app/manager.hpp"
#include <string>
#include <unordered_map>

class Image final {
public:
    friend class Renderer;

    Image(Renderer& renderer, const std::string& filename);
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
