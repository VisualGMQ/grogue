#include "core/tilesheet.hpp"
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
        anim_.SetLoop(-1);
    }

    void Update() override {
        anim_.Update();
    }

    void Render() override {
        grogue::core::Rect rect(100, 100, 100, 50);

        auto& image = anim_.GetCurFrame().image;
        grogue::core::Transform transform;
        transform.SetPos(grogue::core::Vec2(400, 200))
                 .SetScale(grogue::core::Vec2(2, 2));
        grogue::core::GFX::DrawImage(image, transform);

        static bool open = true;
        ImGui::Begin("Operators", &open);
            if (ImGui::Button("Play")) {
                anim_.Play();
            }
            if (ImGui::Button("Stop")) {
                anim_.Stop();
            }
            if (ImGui::Button("Pause")) {
                anim_.Pause();
            }
            if (ImGui::Button("Rewind")) {
                anim_.Rewind();
            }
        ImGui::End();
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
        grogue::core::TextureMgr::Load("./assets/kirby.png", "kirby");
    }

    bool OnQuit() override {
        return true;
    }
};

int main(int , char**) {
    grogue::core::Engine::Init("grogue", 1024, 720, true);
    grogue::core::Engine::RunScence<TestAnimScence>("TestAnimScence");
}
