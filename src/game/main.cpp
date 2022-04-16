#include "core/init.hpp"
#include "core/app.hpp"
#include "core/video.hpp"
using namespace std;

class Game: public grogue::core::App {
public:
    void OnRender() override {
        auto& renderer = grogue::core::VideoMgr::GetMainVideo().renderer;
        renderer->Clear({100, 100, 100, 255});
    }

private:
};

int main(int argc, char** argv) {
    grogue::core::InitSystem("grogue", 1024, 720);
    grogue::core::VideoMgr::CreateVideo("test window", 200, 100).window->Show();

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
        
        grogue::core::VideoMgr::Present();
    }

    grogue::core::QuitSystem();
    return 0;
}

