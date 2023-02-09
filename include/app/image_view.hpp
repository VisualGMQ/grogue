#pragma once

#include "app/fwd.hpp"
#include "core/math.hpp"

class ImageView final {
public:
    ImageView() = default;
    ImageView(ImageHandle, const math::Rect&);

    ImageHandle Handle() const { return handle_; }

    math::Rect Region() const { return region_; }

private:
    ImageHandle handle_;
    math::Rect region_;
};
