#include "core/video.hpp"
#include "core/timer.hpp"
#include "core/engine.hpp"
#include "core/utf8string.hpp"
#include "core/image.hpp"

class MainLayer final: public grogue::core::Layer {
public:
    MainLayer(std::string_view name): grogue::core::Layer(name) {}

    void Render() override {
        auto& renderer = grogue::core::VideoMgr::GetMainVideo()->renderer;
        renderer->Clear(grogue::core::Color(100, 100, 100, 255));

        static auto image = grogue::core::Image::CreateImage("kirby", grogue::core::Recti(0, 0, 36, 36));
        image.SetRotation(image.GetRotation() + 50 * grogue::core::Timer::GetSecondsBetweenFrames());
        image.SetPos(grogue::core::Vec2(100, 100));

        renderer->DrawImage(image);
    }
};

class MainScence: public grogue::core::Scence {
public:
    MainScence(std::string_view name): Scence(name) {
        grogue::core::TextureMgr::Load("./assets/kirby.png", "kirby");
        PushBackLayer<MainLayer>("main-layer");
    }
};

int main(int, char**) {
    grogue::core::Engine::Init("grogue", 1025, 720, false);
    grogue::core::Engine::RunScence<MainScence>("MainScence");

    return 0;
}

