#include "core/mathf.hpp"
#include "core/video.hpp"
#include "core/timer.hpp"
#include "core/engine.hpp"
#include "core/image.hpp"

class TestLayer: public grogue::core::Layer {
public:
    TestLayer(std::string_view name): grogue::core::Layer(name) {}

    void OnInit() override {
        kriby_ = grogue::core::TextureMgr::Find("kriby");
    }

    void Render() override {
        grogue::core::VideoMgr::GetMainVideo()->renderer->DrawImage(
            grogue::core::Image::Create("kriby"),
            grogue::core::Transform().SetPos(grogue::core::Vec2(0, 0))
        );
    }

private:
    grogue::core::Texture* kriby_;
};

class TestScence: public grogue::core::Scence {
public:
    TestScence(std::string_view name): grogue::core::Scence(name) {
        PushBackLayer<TestLayer>("TestLayer");
    }

    void OnInit() override {
        grogue::core::TextureMgr::Load("./assets/kirby.png", "kriby");
    }
};

int main(int, char**) {
    grogue::core::Engine::Init("grogue", 1024, 720);
    grogue::core::Engine::RunScence<TestScence>("TestTextureScence");

    return 0;
}
