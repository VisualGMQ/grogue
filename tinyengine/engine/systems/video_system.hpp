#pragma once

#include "engine/core/pch.hpp"
#include "engine/ecs/system.hpp"
#include "engine/ecs/world.hpp"
#include "engine/core/video.hpp"
#include "engine/core/event.hpp"
#include "engine/renderer/renderer.hpp"
#include "engine/core/init_config.hpp"

namespace engine {

class VideoSystem: public PerFrameSystem {
public:
    VideoSystem(engine::World* world): PerFrameSystem(world) {
        auto& config = InitConfig::Instance();

        uint32_t flags = SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI;
        if (config.Resizable()) {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        SDL_Window* window = SDL_CreateWindow(config.Title().data(),
                                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                              config.WindowSize().w, config.WindowSize().h,
                                              flags);
        if (!window) {
            Loge("window create failed");
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
            Loge("renderer create failed");
        }
        Video::RegistVideo(window, renderer);
    }

    void Update() override {
        engine::Event::ClearState();
        Video::SwapBuffers();
        Renderer::Clear();
        Event::HandleEvent();
    }
};

}