#pragma once

#include "app/image.hpp"
#include "app/lua.hpp"
#include "core/pch.hpp"
#include "app/sprite.hpp"
#include "app/config_parse.hpp"

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Fourway final {
    static Fourway Zero() { return {0, 0, 0, 0}; };

    T left, right, top, bottom;
};

using Margin = Fourway<uint32_t>;

struct Spacing final {
    uint32_t x, y;
};

class Tilesheet final {
public:
    Tilesheet(ImageManager&, ImageHandle, uint32_t col, uint32_t row,
              const Margin& margin = Margin::Zero(),
              const Spacing& spacing = {0, 0});

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
    Spacing spacing_;
    uint32_t tileWidth_;
    uint32_t tileHeight_;
    uint32_t col_;
    uint32_t row_;
};


struct TilesheetConfig final {
    std::string filename;
    std::string name;
    uint32_t row, col;
    Margin margin = Margin::Zero();
    Spacing spacing = {0, 0};
};

class TilesheetManager final {
public:
    TilesheetManager(ImageManager&, LuaManager&);
    Tilesheet& CreateFromImage(
        ImageHandle,
        const std::string& name,
        uint32_t col, uint32_t row,
        const Margin& margin = Margin::Zero(),
        const Spacing& spacing = {0, 0});
    Tilesheet& LoadFromFile(
        const std::string& filename,
        const std::string& name,
        uint32_t col, uint32_t row,
        const Margin& margin = Margin::Zero(),
        const Spacing& spacing = {0, 0});
    Tilesheet& LoadFromConfig(const std::string& configFilename);
    Tilesheet& Find(std::string_view name);

private:
    ImageManager* imageManager_;
    LuaManager* luaManager_;
    std::unordered_map<std::string, Tilesheet> tilesheets_;
};