#include "engine/input/mouse.hpp"

namespace engine {

bool Mouse::isShow_ = true;

void Mouse::Hide() {
    isShow_ = false;
    SDL_ShowCursor(false);
}

void Mouse::Show() {
    isShow_ = true;
    SDL_ShowCursor(true);
}

bool Mouse::IsShowing() {
    return isShow_ || IsRelativeMode();
}

void Mouse::Capture(bool enable) {
    SDL_CaptureMouse(enable ? SDL_TRUE : SDL_FALSE);
}

void Mouse::RelativeMode(bool enable) {
    SDL_SetRelativeMouseMode(enable ? SDL_TRUE : SDL_FALSE);
}

bool Mouse::IsRelativeMode() {
    return SDL_GetRelativeMouseMode();
}

void Mouse::ToggleRelativeMode() {
    if (IsRelativeMode()) {
        RelativeMode(false);
    } else {
        RelativeMode(true);
    }
}

Cursor::Cursor(std::string_view filename, int x, int y, const Color& keyColor) {
    auto surface = SDL_LoadBMP(filename.data());
    SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, keyColor.r, keyColor.g, keyColor.b));
    cursor_ = SDL_CreateColorCursor(surface, x, y);
    SDL_FreeSurface(surface);
}

Cursor::Cursor(SDL_SystemCursor id) {
    cursor_ = SDL_CreateSystemCursor(id);
}

Cursor::~Cursor() {
    SDL_FreeCursor(cursor_);
}

void Mouse::SetCursor(const Cursor& cursor) {
    SDL_SetCursor(cursor.cursor_);
}

void Mouse::ToggleShow() {
    if (IsShowing()) {
        Hide();
    } else {
        Show();
    }
}

}