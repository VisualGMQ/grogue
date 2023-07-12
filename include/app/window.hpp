#pragma once

#include "core/pch.hpp"
#include "core/math.hpp"

class [[refl, luabind(res)]] Window final {
public:
    friend class Renderer;

    Window(const std::string& title, uint32_t w, uint32_t h);
    Window(const Window&) = delete;
    Window(Window&&);
    ~Window();

    Window& operator=(Window&&);
    Window& operator=(const Window&) = delete;

    math::Vector2 GetSize() const;

    SDL_Window* [[luanobind]] Raw() const { return window_; }

private:
    SDL_Window* window_ = nullptr;

    friend void swap(Window& lhs, Window& rhs) noexcept {
        std::swap(lhs.window_, rhs.window_);
    }
};