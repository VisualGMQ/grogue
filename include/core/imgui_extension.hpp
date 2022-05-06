#pragma once

#include "mathf.hpp"
#include "image.hpp"

namespace ImGui {

#define Vec2Im(vec) ImVec2(vec.x, vec.y)

inline void ImageEXT(const grogue::core::Image& image, const grogue::core::Vec2& size) {
    ImGui::Image(image.GetRawTexture(),
                 Vec2Im(size),
                 Vec2Im(image.GetUV1()),
                 Vec2Im(image.GetUV2()));
}

inline void ImageEXT(const grogue::core::Image& image) {
    ImGui::Image(image.GetRawTexture(),
                 Vec2Im(image.GetSize()),
                 Vec2Im(image.GetUV1()),
                 Vec2Im(image.GetUV2()));
}

#undef Vec2Im

}
