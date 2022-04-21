#pragma once

#include "pch.hpp"
#include "log.hpp"
#include "utf8string.hpp"
#include "mathf.hpp"
#include "storage.hpp"

namespace grogue::core {

class Font final {
public:
    friend class Renderer;

    Font(const char* filename, int pt);
    ~Font();

    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;

    Size GetSize(const char*);
    Size GetSize(const std::string&);

private:
    TTF_Font* font_;
};


class FontMgr final {
public:
    static Font* Create(const std::string& filename, int pt);
    static Font* Create(const std::string& filename,
                        const std::string& name, int pt);
    static Font* Find(const std::string& name);

private:
    static Storage<std::string, std::unique_ptr<Font>> fonts_;
};

}
