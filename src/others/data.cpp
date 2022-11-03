#include "data.hpp"
#include "controller/human_controller.hpp"

std::unique_ptr<GameData> GameData::instance_ = nullptr;

GameData* GameData::Instance() {
    if (!instance_) {
        instance_ = std::make_unique<GameData>();
    }
    return instance_.get();
}

void GameData::InitControllers(engine::Entity* player) {
    constexpr float speed = 0.2;
    humanController_ = std::make_unique<HumanController>(player);
    humanController_->SetUpBtn(std::make_unique<controller::keyboard::MoveButton>(humanController_.get(), engine::Vec2(0, -speed), SDL_SCANCODE_W));
    humanController_->SetDownBtn(std::make_unique<controller::keyboard::MoveButton>(humanController_.get(), engine::Vec2(0, speed), SDL_SCANCODE_S));
    humanController_->SetLeftBtn(std::make_unique<controller::keyboard::MoveButton>(humanController_.get(), engine::Vec2(-speed, 0), SDL_SCANCODE_A));
    humanController_->SetRightBtn(std::make_unique<controller::keyboard::MoveButton>(humanController_.get(), engine::Vec2(speed, 0), SDL_SCANCODE_D));
    humanController_->SetPickupBtn(std::make_unique<controller::keyboard::PickupButton>(humanController_.get(), SDL_SCANCODE_U));
    humanController_->SetOpenBackpackPanelBtn(std::make_unique<controller::keyboard::OpenBackpackButton>(humanController_.get(), SDL_SCANCODE_E));
    humanController_->SetOpenCompositePanelBtn(std::make_unique<controller::keyboard::OpenCompositePanel>(humanController_.get(), SDL_SCANCODE_TAB));
    humanController_->SetPutLeftBtn(std::make_unique<controller::keyboard::PutButton>(humanController_.get(),
                                                                                      SDL_SCANCODE_J,
                                                                                      controller::keyboard::PutButton::HandType::Left));
    humanController_->SetPutRightBtn(std::make_unique<controller::keyboard::PutButton>(humanController_.get(),
                                                                                       SDL_SCANCODE_K,
                                                                                       controller::keyboard::PutButton::HandType::Right));

    backpackController_ = std::make_unique<BackpackController>();
    auto backpackPanel = Instance()->GetBackpackPanel()->GetComponent<component::GridPanel>();
    backpackController_->SetUpBtn(std::make_unique<controller::keyboard::GridPanelMoveUpButton>(backpackPanel, SDL_SCANCODE_W));
    backpackController_->SetDownBtn(std::make_unique<controller::keyboard::GridPanelMoveDownButton>(backpackPanel, SDL_SCANCODE_S));
    backpackController_->SetLeftBtn(std::make_unique<controller::keyboard::GridPanelMoveLeftButton>(backpackPanel, SDL_SCANCODE_A));
    backpackController_->SetRightBtn(std::make_unique<controller::keyboard::GridPanelMoveRightButton>(backpackPanel, SDL_SCANCODE_D));
    backpackController_->SetCloseBackpackBtn(std::make_unique<controller::keyboard::CloseBackpackButton>(SDL_SCANCODE_E));
    backpackController_->SetLeftHandSelectBtn(std::make_unique<controller::keyboard::LeftHandSelectButton>(SDL_SCANCODE_J));
    backpackController_->SetRightHandSelectBtn(std::make_unique<controller::keyboard::RightHandSelectButton>(SDL_SCANCODE_K));

    compositeController_ = std::make_unique<CompositeController>();
    auto compositePanel = Instance()->GetCompositePanel()->GetComponent<component::GridPanel>();
    compositeController_->SetUpBtn(std::make_unique<controller::keyboard::GridPanelMoveUpButton>(compositePanel, SDL_SCANCODE_W));
    compositeController_->SetDownBtn(std::make_unique<controller::keyboard::GridPanelMoveDownButton>(compositePanel, SDL_SCANCODE_S));
    compositeController_->SetLeftBtn(std::make_unique<controller::keyboard::GridPanelMoveLeftButton>(compositePanel, SDL_SCANCODE_A));
    compositeController_->SetRightBtn(std::make_unique<controller::keyboard::GridPanelMoveRightButton>(compositePanel, SDL_SCANCODE_D));
    compositeController_->SetCloseBtn(std::make_unique<controller::keyboard::CloseCompositePanelButton>(SDL_SCANCODE_TAB));
    compositeController_->SetComposeBtn(std::make_unique<controller::keyboard::ComposeButton>(SDL_SCANCODE_J));

    controller_ = GetHumanController();
}

engine::Entity* GameData::GetBackpackHoverObject() {
    auto player = GetPlayer();
    if (!player) return nullptr;

    auto backpack = player->GetComponent<component::Backpack>();
    if (!backpack) return nullptr;

    auto backpackPanel = GetBackpackPanel();
    if (!backpackPanel) return nullptr;

    auto index = backpackPanel->GetComponent<component::GridPanel>()->GetHoverIndex();

    if (backpack->objects.size() <= index || index < 0) return nullptr;

    return backpack->objects[index];
}


std::vector<engine::Entity*> MonsterManager::monsters_;
bool MonsterManager::shouldSort_ = true;