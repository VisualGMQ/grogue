#include "core/app.hpp"

namespace grogue::core {

void App::OnEvent(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        Exit();
    }
    if (e.type == SDL_WINDOWEVENT) {
        if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
            if (e.window.windowID == VideoMgr::GetMainVideo().window->GetID()) {
                Exit();
            } else {
                VideoMgr::FindByID(e.window.windowID).window->Hide();
            }
        }
    }
}

}
