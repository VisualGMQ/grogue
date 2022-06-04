#pragma once

#include "pch.hpp"
#include "scence.hpp"
#include "err_code.hpp"
#include "timer.hpp"
#include "video.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "gfx.hpp"

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
    static Size initWindowSize_;

    static void initImGui();
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

    Timer::SteadyTimer.Record();
    while (!ShouldQuit()) {
        Keyboard::SwapKeyState();
        Mouse::SwapButtonStates();

        while (SDL_PollEvent(&event)) {
            Keyboard::AccpetEvent(event);
            Mouse::AcceptEvent(event);
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT) {
                if (scence && scence->OnQuit()) {
                    Exit();
                }
            }
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_TAKE_FOCUS) {
                    Mouse::SetFocusWindow(VideoMgr::FindByID(event.window.windowID)->window.get());
                }
            }
            if (scence) {
                scence->OnEventHandle(event);
            }
        }

        VideoMgr::ClearScreen();

        Timer::SteadyTimer.Record();

        ImGui_ImplSDL2_NewFrame();
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui::NewFrame();

        if (scence) {
            scence->OnUpdate();
            scence->OnRender();
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

        VideoMgr::Present();
    }
    cleanUp();
}

}
