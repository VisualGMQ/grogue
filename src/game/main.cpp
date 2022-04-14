#include "core/init.hpp"
#include "core/app.hpp"
#include "core/video.hpp"
using namespace std;

class Game: public grogue::core::App {
public:
    void OnRender() override {
        auto& renderer = grogue::core::VideoMgr::GetVideo().renderer;
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
    }

private:
};

int main(int argc, char** argv) {
    grogue::core::InitSystem("grogue", 1024, 720);

    Game game;

    SDL_Event event;

    auto t = chrono::steady_clock::now();
    while (!game.ShouldQuit()) {
        while (SDL_PollEvent(&event)) {
            game.OnEvent(event);
        }

        auto currentTime = chrono::steady_clock::now();
        auto elapse = chrono::duration_cast<chrono::milliseconds>(currentTime - t);
        t = currentTime;

        game.OnUpdate(elapse.count());
        game.OnRender();
        SDL_RenderPresent(grogue::core::VideoMgr::GetVideo().renderer);
    }

    grogue::core::QuitSystem();
    return 0;
}

