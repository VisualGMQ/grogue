#include "backpack_controller.hpp"
#include "others/data.hpp"
#include "controller/human_controller.hpp"

void BackpackController::Update() {
    if (up_) up_->Update();
    if (down_) down_->Update();
    if (right_) right_->Update();
    if (left_) left_->Update();
    if (closeBackpack_) closeBackpack_->Update();
}