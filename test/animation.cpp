#include "core/tilesheet.hpp"
#include "gui/init.hpp"
#include "gui/widget.hpp"
#include "core/scence.hpp"
#include "core/layer.hpp"
#include "core/engine.hpp"
#include "core/log.hpp"
#include "core/animation.hpp"

class TestAnimLayer: public grogue::core::Layer {
public:
    TestAnimLayer(std::string_view name): grogue::core::Layer(name) {}

    void OnInit() override {
        delayTime_ = 100;
        tilesheet_.reset(new grogue::core::TileSheet(grogue::core::Image::Create("kirby"), 16, 2));
        anim_ = grogue::core::Animation::Create(*tilesheet_, std::vector<uint32_t>(tilesheet_->GetCol(), delayTime_), 0);
        anim_.SetLoop(1);
    }

    void Update() override {
        anim_.Update();
    }

    void Render() override {
        bool show = true;
        grogue::core::Rect rect(100, 100, 100, 50);
        if (grogue::gui::Button(show, "Play", &rect)) {
            anim_.Play();
        }
        rect.x += 100;
        if (grogue::gui::Button(show, "Pause", &rect)) {
            anim_.Pause();
        }
        rect.x += 100;
        if (grogue::gui::Button(show, "Stop", &rect)) {
            anim_.Stop();
        }
        rect.x += 100;
        if (grogue::gui::Button(show, "Rewind", &rect)) {
            anim_.Rewind();
        }

        auto& image = anim_.GetCurFrame().image;
        grogue::core::Transform transform;
        transform.SetPos(grogue::core::Vec2(400, 200))
                 .SetScale(grogue::core::Vec2(2, 2));
        grogue::core::VideoMgr::GetMainVideo()->renderer->DrawImage(image, transform);
    }

private:
    std::unique_ptr<grogue::core::TileSheet> tilesheet_;
    grogue::core::Animation anim_;
    uint32_t delayTime_;
};

class TestAnimScence: public grogue::core::Scence {
public:
    TestAnimScence(std::string_view name): grogue::core::Scence(name) {
        PushBackLayer<TestAnimLayer>("test_anim_layer");
    }

    void OnInit() override {
        grogue::gui::Init(grogue::core::FontMgr::Create("./assets/Perpetua.ttf", 20));
        grogue::core::TextureMgr::Load("./assets/kirby.png", "kirby");
    }

    bool OnQuit() override {
        grogue::gui::Quit();
        LOG_INFO("gui quited");
        return true;
    }
};

int main(int , char**) {
    grogue::core::Engine::Init("grogue", 1024, 720);
    grogue::core::Engine::RunScence<TestAnimScence>("TestAnimScence");
}
