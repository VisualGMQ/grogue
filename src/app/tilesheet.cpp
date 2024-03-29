#include "app/tilesheet.hpp"
#include "app_tilesheet_refl.hpp"

Tilesheet Tilesheet::Null;

Tilesheet::Tilesheet(ImageManager& manager, ImageHandle handle, uint32_t col,
                     uint32_t row, const Margin& margin,
                     const Spacing& spacing)
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

Tile Tilesheet::Get(uint32_t x, uint32_t y) {
    return Tile{
        math::Rect{
                   static_cast<float>(x * (tileWidth_ + spacing_.x) + margin_.left),
                   static_cast<float>(y * (tileHeight_ + spacing_.y) + margin_.top),
                   static_cast<float>(tileWidth_), static_cast<float>(tileHeight_)},
        handle_
    };
}

Tile Tilesheet::Get(uint32_t index) {
    return Get(index % col_, index / col_);
}

TilesheetManager::TilesheetManager(ImageManager& imageMgr, LuaManager& luaMgr)
    : imageManager_(&imageMgr), luaManager_(&luaMgr) {}

Tilesheet& TilesheetManager::CreateFromImage(ImageHandle handle,
                                             const std::string& name,
                                             uint32_t col,
                                             uint32_t row, const Margin& margin,
                                             const Spacing& spacing) {
    auto it = tilesheets_.insert_or_assign(name, Tilesheet(*imageManager_, handle, col, row, margin, spacing));
    return it.first->second;
}

Tilesheet& TilesheetManager::LoadFromFile(const std::string& filename,
                                          const std::string& name,
                                          uint32_t col, uint32_t row,
                                          const Margin& margin,
                                          const Spacing& spacing) {
    auto handle = imageManager_->Load(filename);
    return CreateFromImage(handle, name, col, row, margin, spacing);
}

Tilesheet& TilesheetManager::LoadFromConfig(const std::string& configFilename) {
    auto lua = luaManager_->CreateSolitary(configFilename);
    auto table = lua.lua.get<sol::table>("Config");

    auto config = serialize::DeserializeFromLua<TilesheetConfig>(table);

    std::string filename = configFilename.substr(0, configFilename.find_last_of('/') + 1) + config.value().filename.substr(config.value().filename.find_last_of('/'));

    return LoadFromFile(filename,
                        config.value().name,
                        config.value().col,
                        config.value().row, config.value().margin,
                        config.value().spacing);
}

Tilesheet& TilesheetManager::Find(std::string_view name) {
    if (auto it = tilesheets_.find(std::string(name)); it != tilesheets_.end()) {
        return it->second;
    } else {
        LOGW("tilesheet ", name, " not found");
        return Tilesheet::Null;
    }
}