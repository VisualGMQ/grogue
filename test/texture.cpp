#include "core/video.hpp"
#include "core/timer.hpp"
#include "core/texture.hpp"
#include "core/engine.hpp"

class TestScence: public grogue::core::Scence {
public:
    TestScence(std::string_view name): grogue::core::Scence(name) {}

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
    grogue::core::Engine::Init("grogue", 1024, 720);
    grogue::core::Engine::RunScence<TestScence>("TestTextureScence");

    return 0;
}

