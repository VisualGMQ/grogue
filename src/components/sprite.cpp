#include "sprite.hpp"

namespace component {

void Sprite::Reset() {
    offset.Set(0, 0);
    size.Set(0, 0);
    anchor.Set(0, 0);
    rotation = 0;
    image.texture = nullptr;
}

}