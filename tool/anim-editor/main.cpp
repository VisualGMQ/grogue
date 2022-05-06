#include "core/animation.hpp"
#include "core/engine.hpp"
#include "core/imgui_extension.hpp"
#include "ImGuiFileDialog.h"
using namespace grogue::core;

class MainLayer: public Layer {
public:
    MainLayer(std::string_view name): Layer(name) {}

    void Render() override {
        Image image = Image::Create("kirby");
        ImGui::Begin("image show");
            ImGui::Text("image size: %d, %d", image.GetSize().w, image.GetSize().h);
            ImGui::ImageEXT(image);
        ImGui::End();
        bool show = true;
        ImGui::ShowDemoWindow(&show);

        if (ImGui::Button("Open File Dialog"))
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");

        // display
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
            }

            // close
            ImGuiFileDialog::Instance()->Close();
        }
    }

private:
    std::vector<Frame> frames_;
};

class MainScence: public Scence {
    public:
        MainScence(std::string_view name): Scence(name) {}

        void OnInit() override {
            PushBackLayer<MainLayer>("MainLayer");
            TextureMgr::Load("./assets/kirby.png", "kirby");

        }
};

int main(int, char**) {
    grogue::core::Engine::Init("grogue", 1025, 720, true);
    grogue::core::Engine::RunScence<MainScence>("MainScence");
    return 0;
}
