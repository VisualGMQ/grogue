#include "tilesheet_loader.hpp"

void LoadImageResources(const std::filesystem::path& root, const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) return;
    if (!std::filesystem::is_directory(path)) {
        Loge("path \"{}\" isn't directory", path);
        return;
    }
    std::filesystem::directory_iterator dirIter(path);
    while (dirIter != std::filesystem::directory_iterator()) {
        if (dirIter->is_directory()) {
            LoadImageResources(root, dirIter->path());
        } else if (dirIter->is_regular_file() && IsImageFile(dirIter->path())) {
            std::filesystem::path configPath;
            if (ExistsTileesheetConfigFile(dirIter->path(), configPath)) {
                LoadTilesheet(root, dirIter->path(), configPath);
            } else {
                LoadImage(root, dirIter->path());
            }
        }
        dirIter++;
    }
}

bool ExistsTileesheetConfigFile(const std::filesystem::path& tilesheet, OUT std::filesystem::path& outConfigFilename) {
    if (tilesheet.extension() == ".toml") return false;
    outConfigFilename = GetFilenameNoExt(tilesheet.string()) + ".toml";
    return std::filesystem::exists(outConfigFilename);
}

void LoadImage(const std::filesystem::path& root, const std::filesystem::path& filepath) {
    std::string name = RemoveRootFromPath(root.string(), GetFilenameNoExt(filepath.string()));
    CvtWindowsDelim2Unix(name);
    auto texture = engine::TextureFactory::Create(filepath.string());
    engine::ImageFactory::Create(texture, name, std::nullopt);
}

void LoadTilesheet(const std::filesystem::path& root,
                   const std::filesystem::path& tilesheetPath,
                   const std::filesystem::path& configPath) {
    engine::Texture* texture = engine::TextureFactory::Create(tilesheetPath.string());
    if (!texture) {
        Loge("tilesheet {} load failed", tilesheetPath);
        return ;
    }

    auto tbl = toml::parse_file(configPath.string());
    int row = tbl["row"].value_or(0);
    int col = tbl["col"].value_or(0);
    if (row == 0 || col == 0) {
        Loge("tilesheet \"{}\" config invalid: row == 0 or col == 0", configPath);
        return ;
    }
    if (!tbl["content"].is_table()) {
        Loge("tilesheet \"{}\" no content");
        return ;
    }
    auto table = tbl["content"].as_table();
    if (!table) {
        Loge("tilesheet \"{}\" no content");
        return ;
    }

    engine::Size tileSize(texture->Width() / col, texture->Height() / row);
    for (auto& elem : *table) {
        auto arr = elem.second.as_array();
        if (!arr) {
            Loge("tilesheet \"{}\" config: content's element not contain array", configPath);
        } else {
            std::string filepath = GetFilenameNoExt(tilesheetPath.string());
            CvtWindowsDelim2Unix(filepath);
            std::string name = RemoveRootFromPath(root.string(), filepath + "#" + std::string(elem.first.str()));

            int x = arr->at(0).as_integer()->value_or(-1);
            int y = arr->at(1).as_integer()->value_or(-1);
            if (x == -1 || y == -1) {
                Loge("tilesheet \"{}\" config: content's x or y is -1", configPath);
            }
            engine::ImageFactory::Create(texture, name, engine::Rect(tileSize.w * x, tileSize.h * y, tileSize.w, tileSize.h));
        }
    }
}

bool IsImageFile(const std::filesystem::path& filepath) {
    if (!filepath.has_extension()) return false;
    auto extension = filepath.extension();
    return extension == ".png" ||
           extension == ".jpg" ||
           extension == ".jpeg" ||
           extension == ".bmp" ||
           extension == ".xmp";
}