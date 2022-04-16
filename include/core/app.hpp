#pragma once

#include "core/pch.hpp"
#include "core/video.hpp"

namespace grogue::core {

class App {
public:
    App() = default;
    virtual ~App() = default;

    virtual void OnInit() {}
    virtual void OnUpdate(std::uint32_t) {}
    virtual void OnRender() {}
    virtual void OnQuit() {}
    virtual void OnEvent(SDL_Event& e);
    bool ShouldQuit() const { return shouldQuit_; }
    void Exit() { shouldQuit_ = true; }

private:
    bool shouldQuit_ = false;
};

}
