#include "app/image_view.hpp"

ImageView::ImageView(ImageHandle handle, const math::Rect& region)
    : handle_(handle), region_(region) {}