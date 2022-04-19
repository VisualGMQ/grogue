#include "core/video.hpp"
#include "core/timer.hpp"
#include "core/engine.hpp"

using namespace std;

class MainLayer final: public grogue::core::Layer {
public:
    MainLayer(std::string_view name): grogue::core::Layer(name) {}

    void Render() override {
        auto& renderer = grogue::core::VideoMgr::GetMainVideo().renderer;
        renderer->Clear(grogue::core::Color(100, 100, 100, 255));
    }
};

class MainScence: public grogue::core::Scence {
public:
    MainScence(std::string_view name): Scence(name) {
        PushBackLayer<MainLayer>("main-layer");
    }
};

int main(int, char**) {
    grogue::core::Engine::Init("grogue", 1025, 720, false);
    grogue::core::Engine::RunScence<MainScence>("MainScence");

    return 0;
}

