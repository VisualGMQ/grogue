#include "scenes/start_scene.hpp"

void StartScene::OnInit() {
    loadResources();
    LoadImageResources("./resources", "./resources");
}

void StartScene::OnQuit() {

}

void StartScene::loadResources() {
}