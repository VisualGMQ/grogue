#pragma once

#include "engine/engine.hpp"
#include "controller.hpp"
#include "button.hpp"
#include "ui/grid_panel.hpp"

class CompositeController: public controller::Controller {
public:
    void Update() override;

    void SetUpBtn(std::unique_ptr<controller::Button>&& button) { up_ = std::move(button); }
    void SetDownBtn(std::unique_ptr<controller::Button>&& button) { down_ = std::move(button); }
    void SetLeftBtn(std::unique_ptr<controller::Button>&& button) { left_ = std::move(button); }
    void SetRightBtn(std::unique_ptr<controller::Button>&& button) { right_ = std::move(button); }
    void SetComposeBtn(std::unique_ptr<controller::Button>&& button) { compose_ = std::move(button); }
    void SetCloseBtn(std::unique_ptr<controller::Button>&& button) { closePanel_ = std::move(button); }

private:
    std::unique_ptr<controller::Button> up_;
    std::unique_ptr<controller::Button> down_;
    std::unique_ptr<controller::Button> left_;
    std::unique_ptr<controller::Button> right_;
    std::unique_ptr<controller::Button> compose_;
    std::unique_ptr<controller::Button> closePanel_;

    void changeDescriptionPanelCapacity(component::GridPanel* panel);
};
