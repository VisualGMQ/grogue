#pragma once

#include "engine/core/pch.hpp"

namespace engine {

using FontID = unsigned int;

class Font final {
public:
    friend class Renderer;

    Font(FontID id, const std::string& filename, const std::string& name, int pt);
    ~Font();

    Vec2 SizeUTF8(const std::string& text) const;
    int Pt() const { return pt_; }

    const std::string& Name() const { return name_; }

private:
    TTF_Font* font_;
    int pt_;
    FontID id_;
    std::string name_;
};

class FontFactory final {
public:
    static void Init();
    static void Quit();

    static Font* Find(FontID id);
    static Font* Find(const std::string& name);
    static Font* Create(const std::string& filename, const std::string& name, int pt);

private:
    static std::unordered_map<FontID, std::unique_ptr<Font>> fontMap_;
    static FontID curId_;
};

}