#pragma once

namespace grogue::core {

enum ExitCode: int {
    NO_ERROR = 0,
    SDL_INIT_FAILED,
    IMG_INIT_FAILED,
    TTF_INIT_FAILED,
};

}
