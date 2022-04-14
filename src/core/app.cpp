#include "core/app.hpp"

namespace grogue::core {

void App::OnEvent(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        Exit();
    }
}

}
