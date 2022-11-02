#pragma once

#include "controller.hpp"
#include "button.hpp"

class BackpackController: public controller::Controller {
public:
    BackpackController() = default;

    void SetUpBtn(std::unique_ptr<controller::Button>&& btn) { up_ = std::move(btn); }
    void SetDownBtn(std::unique_ptr<controller::Button>&& btn) { down_ = std::move(btn); }
    void SetLeftBtn(std::unique_ptr<controller::Button>&& btn) { left_ = std::move(btn); }
    void SetRightBtn(std::unique_ptr<controller::Button>&& btn) { right_ = std::move(btn); }
    void SetCloseBackpackBtn(std::unique_ptr<controller::Button>&& btn) { closeBackpack_ = std::move(btn); }
    void SetLeftHandSelectBtn(std::unique_ptr<controller::Button>&& btn) { leftHandSelect_= std::move(btn); }
    void SetRightHandSelectBtn(std::unique_ptr<controller::Button>&& btn) { rightHandSelect_= std::move(btn); }

    void Update() override;

private:
    std::unique_ptr<controller::Button> up_;
    std::unique_ptr<controller::Button> down_;
    std::unique_ptr<controller::Button> left_;
    std::unique_ptr<controller::Button> right_;
    std::unique_ptr<controller::Button> closeBackpack_;
    std::unique_ptr<controller::Button> leftHandSelect_;
    std::unique_ptr<controller::Button> rightHandSelect_;
};