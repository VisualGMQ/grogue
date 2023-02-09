#pragma once

#include "app/image_view.hpp"
#include "app/image.hpp"
#include "core/pch.hpp"

class TileSheet final {
public:
    TileSheet(ImageManager&, ImageHandle, uint32_t col, uint32_t row, uint32_t xPadding = 0, uint32_t yPadding = 0);

    operator bool() const {
        return handle_ && tileWidth_ > 0 && tileHeight_ > 0;
    }

    ImageView Get(uint32_t x, uint32_t y);
    ImageView Get(uint32_t index);

private:
    ImageHandle handle_;
    uint32_t xPadding_;
    uint32_t yPadding_;
    uint32_t tileWidth_;
    uint32_t tileHeight_;
    uint32_t col_;
    uint32_t row_;
};

class TileSheetManager final {
public:
    TileSheetManager(ImageManager&);
    TileSheet& CreateFromImage(ImageHandle, uint32_t col,
                               uint32_t row, uint32_t xPadding = 0,
                               uint32_t yPadding = 0);
    TileSheet& LoadFromFile(const std::string& filename, uint32_t col,
                            uint32_t row, uint32_t xPadding = 0,
                            uint32_t yPadding = 0);

private:
    ImageManager* manager_;
    std::vector<TileSheet> tilesheets_;
};