#pragma once

#include "app/image.hpp"
#include "app/lua.hpp"
#include "core/pch.hpp"
#include "app/sprite.hpp"

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Fourway {
    static Fourway Zero() { return {0, 0, 0, 0}; };

    T left, right, top, bottom;
};

using Margin = Fourway<uint32_t>;

class TileSheet final {
public:
    TileSheet(ImageManager&, ImageHandle, uint32_t col, uint32_t row,
              const Margin& margin = Margin::Zero(),
              const math::Vector2& spacing = math::Vector2::Zero);

    operator bool() const {
        return handle_ && tileWidth_ > 0 && tileHeight_ > 0;
    }

    Tile Get(uint32_t x, uint32_t y);
    Tile Get(uint32_t index);
    uint32_t Row() const { return row_; }
    uint32_t Col() const { return col_; }
    ImageHandle Handle() const { return handle_; }
    math::Vector2 TileSize() const {
        return {static_cast<float>(tileWidth_), static_cast<float>(tileHeight_)};
    }

private:
    ImageHandle handle_;
    Margin margin_;
    math::Vector2 spacing_;
    uint32_t tileWidth_;
    uint32_t tileHeight_;
    uint32_t col_;
    uint32_t row_;
};

class TileSheetManager final {
public:
    TileSheetManager(ImageManager&, LuaManager&);
    TileSheet& CreateFromImage(
        ImageHandle, uint32_t col, uint32_t row,
        const Margin& margin = Margin::Zero(),
        const math::Vector2& spacing = math::Vector2::Zero);
    TileSheet& LoadFromFile(
        const std::string& filename, uint32_t col, uint32_t row,
        const Margin& margin = Margin::Zero(),
        const math::Vector2& spacing = math::Vector2::Zero);
    TileSheet& LoadFromConfig(const std::string& configFilename);

private:
    ImageManager* imageManager_;
    LuaManager* luaManager_;
    std::vector<TileSheet> tilesheets_;
};