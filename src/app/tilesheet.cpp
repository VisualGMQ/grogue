#include "app/tilesheet.hpp"

TileSheet::TileSheet(ImageManager& manager, ImageHandle handle, uint32_t col,
                     uint32_t row, const Margin& margin,
                     const math::Vector2& spacing)
    : handle_(handle),
      margin_(margin),
      spacing_(spacing),
      row_(row),
      col_(col) {
    if (handle) {
        auto& image = manager.Get(handle);
        tileWidth_ =
            (image.W() - margin.left - margin.right - (col - 1) * spacing.x) /
            col;
        tileHeight_ =
            (image.H() - margin.top - margin.bottom - (row - 1) * spacing.y) /
            row;
    }
}

Tile TileSheet::Get(uint32_t x, uint32_t y) {
    return Tile{
        math::Rect{
                   static_cast<float>(x * (tileWidth_ + spacing_.x) + margin_.left),
                   static_cast<float>(y * (tileHeight_ + spacing_.y) + margin_.top),
                   static_cast<float>(tileWidth_), static_cast<float>(tileHeight_)},
        handle_
    };
}

Tile TileSheet::Get(uint32_t index) {
    return Get(index % col_, index / col_);
}

TileSheetManager::TileSheetManager(ImageManager& imageMgr, LuaManager& luaMgr)
    : imageManager_(&imageMgr), luaManager_(&luaMgr) {}

TileSheet& TileSheetManager::CreateFromImage(ImageHandle handle, uint32_t col,
                                             uint32_t row, const Margin& margin,
                                             const math::Vector2& spacing) {
    tilesheets_.emplace_back(*imageManager_, handle, col, row, margin, spacing);
    return tilesheets_.back();
}

TileSheet& TileSheetManager::LoadFromFile(const std::string& filename,
                                          uint32_t col, uint32_t row,
                                          const Margin& margin,
                                          const math::Vector2& spacing) {
    auto handle = imageManager_->Load(filename);
    return CreateFromImage(handle, col, row, margin, spacing);
}

TileSheet& TileSheetManager::LoadFromConfig(const std::string& configFilename) {
    auto lua = luaManager_->CreateSolitary(configFilename);
    auto table = lua.lua.get<sol::table>("description");

    std::string filename = table.get<std::string>("filename");

    uint32_t row = table.get<int>("row");
    uint32_t col = table.get<int>("col");

    auto marginTable = table.get<sol::table>("margin");
    Margin margin;
    margin.left = marginTable.get<int>(1);
    margin.right = marginTable.get<int>(2);
    margin.top = marginTable.get<int>(3);
    margin.bottom = marginTable.get<int>(4);

    auto spacingTable = table.get<sol::table>("spacing");
    math::Vector2 spacing{spacingTable.get<float>(1),
                          spacingTable.get<float>(2)};

    return LoadFromFile(filename, col, row, margin, spacing);
}