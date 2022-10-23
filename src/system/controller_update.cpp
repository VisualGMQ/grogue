#include "controller_update.hpp"


void ControllerUpdateSystem::Update() {
    auto controller = GameData::Instance()->GetController();
    if (controller) {
        controller->Update();
    }
}