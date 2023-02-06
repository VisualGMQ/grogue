#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "app/handle.hpp"
#include "app/renderer.hpp"
#include "core/assert.hpp"
#include "app/renderer.hpp"
#include <string>

class Image;

using ImageHandle = Handle<Image>;

class Image final {
public:
    Image(Renderer& renderer, const std::string& filename);
    ~Image();

private:
    ImageHandle handle_;
    SDL_Texture* texture_;
};