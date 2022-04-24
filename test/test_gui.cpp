#include "gui/init.hpp"
#include "gui/widget.hpp"
#include "core/scence.hpp"
#include "core/layer.hpp"
#include "core/engine.hpp"
#include "core/log.hpp"

class TestGUILayer: public grogue::core::Layer {
public:
    TestGUILayer(std::string_view name): grogue::core::Layer(name) {}

    void Update() override {
        if (grogue::core::Mouse::IsPressed(grogue::core::MouseButton::Left)) {
            LOG_WARN("left button clicked");
        }
    }

    void Render() override {
        bool show = true;
        grogue::core::Rect rect(100, 100, 200, 150);
        if (grogue::gui::Button(show, "button", &rect)) {
            LOG_DEBUG("clicked button");
        }
    }
};

class TestGUIScence: public grogue::core::Scence {
public:
    TestGUIScence(std::string_view name): grogue::core::Scence(name) {}

    void OnInit() override {
        grogue::core::Log::SetAllLevel(grogue::core::LogLevel::Trace);

        auto font = grogue::core::FontMgr::Create("assets/Perpetua.ttf", 15);
        grogue::gui::Init(font);
        LOG_INFO("gui init OK");
        PushBackLayer<TestGUILayer>("test_gui_layer");
    }

    bool OnQuit() override {
        grogue::gui::Quit();
        LOG_INFO("gui quited");
        return true;
    }
};

int main(int , char**) {
    grogue::core::Engine::Init("grogue", 1024, 720);
    grogue::core::Engine::RunScence<TestGUIScence>("TestGUIScence");
}
