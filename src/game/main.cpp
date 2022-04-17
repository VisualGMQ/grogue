#include "core/init.hpp"
#include "core/app.hpp"
#include "core/video.hpp"
#include "core/timer.hpp"

using namespace std;

class Game: public grogue::core::App {
public:
    void OnRender() override {
        auto& renderer = grogue::core::VideoMgr::GetMainVideo().renderer;
        renderer->Clear({100, 100, 100, 255});
    }
};

int main(int argc, char** argv) {
    grogue::core::InitSystem("grogue", 1024, 720);

    Game game;

    SDL_Event event;

    while (!game.ShouldQuit()) {
        while (SDL_PollEvent(&event)) {
            game.OnEvent(event);
        }

        grogue::core::Timer::Record();

        game.OnUpdate(grogue::core::Timer::GetMsBetweenFrames());
        game.OnRender();

        grogue::core::VideoMgr::Present();
    }

    grogue::core::QuitSystem();
    return 0;
}

