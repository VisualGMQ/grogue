#include "core/keyboard.hpp"

namespace grogue::core {

bool Keyboard::states_[SDL_NUM_SCANCODES];
bool Keyboard::prevStates_[SDL_NUM_SCANCODES];

void Keyboard::Init() {
    memset(states_, 0, sizeof(states_));
    memset(prevStates_, 0, sizeof(prevStates_));
}

void Keyboard::SwapKeyState() {
    memcpy(prevStates_, states_, sizeof(states_));
}

void Keyboard::AccpetEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        states_[e.key.keysym.scancode] = true;
    }
    if (e.type == SDL_KEYUP) {
        states_[e.key.keysym.scancode] = false;
    }
}

}
