#include "scenes/start_scene.hpp"

void StartScene::OnInit() {
    ObjectConfigStorage::LoadAllConfig("./data/object");
    LoadImageResources("./resources/img", "./resources/img");
}

void StartScene::OnQuit() {

}