#pragma once

#include "pch.hpp"
#include "scence.hpp"
#include "err_code.hpp"
#include "timer.hpp"
#include "video.hpp"

namespace grogue::core {

/* the whole game running code and some state */
class Engine final {
public:
    static void Init(const std::string& title, uint32_t w, uint32_t h, bool resizable = false);

    template <typename ScenceT, typename... Args>
    static void RunScence(std::string_view name, Args&&... args);

    static void Exit();
    static bool ShouldQuit();

private:
    static bool shouldQuit_;

    static void cleanUp();
};

template <typename ScenceT, typename... Args>
void Engine::RunScence(std::string_view name, Args&&... args) {
    ScenceMgr::Init(ScenceMgr::CreateScence<ScenceT>(name, std::forward<Args>(args)...));

    SDL_Event event;

    Scence* scence = ScenceMgr::GetCurrentScence();
    if (!scence) {
        LOG_CRITICAL("Scence Create failed");
    }
    while (!ShouldQuit()) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                if (scence && scence->OnQuit()) {
                    Exit();
                }
            }
            if (scence) {
                scence->OnEventHandle(event);
            }
        }

        Timer::Record();

        if (scence) {
            scence->OnUpdate();
            scence->OnRender();
        }

        VideoMgr::Present();
    }
    cleanUp();
}

}
