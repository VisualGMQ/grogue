#pragma once

#include "keyboard_buttons.hpp"
#include "button.hpp"

class HumanController: public controller::Controller {
public:
    HumanController(engine::Entity* entity);
    void Update() override;

    void SetUpBtn(std::unique_ptr<controller::Button>&& btn) { up_ = std::move(btn); }
    void SetDownBtn(std::unique_ptr<controller::Button>&& btn) { down_ = std::move(btn); }
    void SetLeftBtn(std::unique_ptr<controller::Button>&& btn) { left_ = std::move(btn); }
    void SetRightBtn(std::unique_ptr<controller::Button>&& btn) { right_ = std::move(btn); }
    void SetPickupBtn(std::unique_ptr<controller::Button>&& btn) { pickup_ = std::move(btn); }
    void SetOpenBackpackPanelBtn(std::unique_ptr<controller::Button>&& btn) { openBackpackPanel_ = std::move(btn); }
    void SetOpenCompositePanelBtn(std::unique_ptr<controller::Button>&& btn) { openCompositePanel_ = std::move(btn); }

    engine::Vec2& GetVelocity() { return velocity_; }

private:
    std::unique_ptr<controller::Button> up_;
    std::unique_ptr<controller::Button> down_;
    std::unique_ptr<controller::Button> left_;
    std::unique_ptr<controller::Button> right_;
    std::unique_ptr<controller::Button> pickup_;
    std::unique_ptr<controller::Button> openBackpackPanel_;
    std::unique_ptr<controller::Button> openCompositePanel_;

    engine::Vec2 velocity_;
};