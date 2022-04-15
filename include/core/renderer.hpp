#pragma once

#include "pch.hpp"
#include "window.hpp"
#include "mathf.hpp"

namespace grogue::core {

class Renderer final {
public:
    Renderer(Window& window);
    Renderer(const Renderer&) = delete;
    ~Renderer();

    Renderer& operator=(const Renderer&) = delete;

    void SetDrawColor(const Color&);
    void Clear();
    void Present();

    // TODO add other functions

private:
    SDL_Renderer* renderer_;
};

}
