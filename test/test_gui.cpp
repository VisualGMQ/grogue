#include "gui/init.hpp"
#include "gui/widget.hpp"
#include "core/scence.hpp"
#include "core/layer.hpp"
#include "core/engine.hpp"
#include "core/log.hpp"

class TestGUILayer: public grogue::core::Layer {
public:
    TestGUILayer(std::string_view name): grogue::core::Layer(name) {}

    void Render() override {
        static bool show = true;

        static grogue::gui::Align align = grogue::gui::Align::AlignCenter;

        grogue::core::Rect rect(100, 150, 100, 50);
        if (grogue::gui::Button(show, "left align", &rect)) {
            align = grogue::gui::Align::AlignLeft;
        }

        rect.Reset(300, 150, 100, 50);
        if (grogue::gui::Button(show, "right align", &rect)) {
            align = grogue::gui::Align::AlignRight;
        }

        rect.Reset(200, 100, 100, 50);
        if (grogue::gui::Button(show, "top align", &rect)) {
            align = grogue::gui::Align::AlignTop;
        }

        rect.Reset(200, 200, 100, 50);
        if (grogue::gui::Button(show, "bottom align", &rect)) {
            align = grogue::gui::Align::AlignBottom;
        }

        rect.Reset(200, 150, 100, 50);
        if (grogue::gui::Button(show, "center align", &rect)) {
            align = grogue::gui::Align::AlignCenter;
        }

        rect.Reset(500, 150, 100, 50);
        if (grogue::gui::Button(show, "test button", &rect, align)) {
            LOG_DEBUG("align text");
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
