#include "app/tilesheet.hpp"

TileSheet::TileSheet(ImageManager& manager, ImageHandle handle, uint32_t col,
                     uint32_t row, uint32_t xPadding, uint32_t yPadding)
    : handle_(handle),
      xPadding_(xPadding),
      yPadding_(yPadding),
      row_(row),
      col_(col) {
    if (handle) {
        auto& image = manager.Get(handle);
        tileWidth_ = image.W() / col - xPadding;
        tileHeight_ = image.H() / row - yPadding;
    }
}

ImageView TileSheet::Get(uint32_t x, uint32_t y) {
    return ImageView{
        handle_, math::Rect{(float)x * tileWidth_, (float)y * tileHeight_,
                            (float)tileWidth_, (float)tileHeight_}
    };
}

ImageView TileSheet::Get(uint32_t index) {
    return Get(index % col_, index / col_);
}

TileSheetManager::TileSheetManager(ImageManager& manager)
    : manager_(&manager) {}

TileSheet& TileSheetManager::CreateFromImage(ImageHandle handle, uint32_t col,
                                             uint32_t row, uint32_t xPadding,
                                             uint32_t yPadding) {
    tilesheets_.emplace_back(*manager_, handle, col, row, xPadding, yPadding);
    return tilesheets_.back();
}

TileSheet& TileSheetManager::LoadFromFile(const std::string& filename,
                                          uint32_t col, uint32_t row,
                                          uint32_t xPadding,
                                          uint32_t yPadding) {
    auto handle = manager_->Load(filename);
    return CreateFromImage(handle, col, row, xPadding, yPadding);
}
