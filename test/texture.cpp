#include "core/init.hpp"
#include "core/app.hpp"
#include "core/video.hpp"
#include "core/timer.hpp"
#include "core/texture.hpp"

class Test final : public grogue::core::App {
public:
    void OnInit() override {
        grogue::core::TextureManager::Instance().Load("./assets/kirby.png", "kirby",
                                        *grogue::core::VideoMgr::GetMainVideo().renderer.get());
    }

    void OnRender() override {
        grogue::core::TextureManager::Instance().DrawFrame("kirby", 0, 0, 36, 33, 1, frame,
                                             *grogue::core::VideoMgr::GetMainVideo().renderer.get());
        if (frame >= 15) {
            frame = 0;
        }
        else {
            ++frame;
        }
    }

private:
    int frame = 0;
};

int main(int argc, char** argv) {
    grogue::core::InitSystem("grogue", 1024, 720);

    Test test;

    SDL_Event event;

    test.OnInit();
    while (!test.ShouldQuit()) {
        while (SDL_PollEvent(&event)) {
            test.OnEvent(event);
        }

        grogue::core::Timer::Record();

        test.OnUpdate(grogue::core::Timer::GetMsBetweenFrames());
        test.OnRender();

        grogue::core::VideoMgr::Present();
    }
    test.OnQuit();

    grogue::core::QuitSystem();
    return 0;
}

