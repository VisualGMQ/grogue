#include "core/tilesheet.hpp"
#include "core/video.hpp"
#include "core/timer.hpp"
#include "core/engine.hpp"
#include "core/utf8string.hpp"
#include "core/image.hpp"
#include "core/animation.hpp"
#include "core/gfx.hpp"

class MainLayer final: public grogue::core::Layer {
public:
    MainLayer(std::string_view name): grogue::core::Layer(name) {}

    void OnInit() override {
        tilesheet_.reset(new grogue::core::TileSheet(grogue::core::Image::Create(*grogue::core::TextureMgr::Find("kirby")), 16, 2));
        anim_ = grogue::core::Animation::Create(*tilesheet_, {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, 0);
        anim_.SetLoop(-1);
        anim_.Play();
    }

    void Update() override {
        anim_.Update();
    }

    void Render() override {
        grogue::core::GFX::Clear(grogue::core::Color(100, 100, 100, 255));

        auto& frame = anim_.GetCurFrame();

        grogue::core::Transform transform;

        transform.SetPos(grogue::core::Vec2(100, 100));

        grogue::core::GFX::DrawImage(frame.image, transform);
    }

private:
    std::unique_ptr<grogue::core::TileSheet> tilesheet_;
    grogue::core::Animation anim_;
};

class MainScence: public grogue::core::Scence {
public:
    MainScence(std::string_view name): Scence(name) {
        grogue::core::TextureMgr::Load("./assets/kirby.png", "kirby");
        PushBackLayer<MainLayer>("main-layer");
    }
};

int main(int, char**) {
    grogue::core::Engine::Init("grogue", 1025, 720, true);
    grogue::core::Engine::RunScence<MainScence>("MainScence");

    return 0;
}

