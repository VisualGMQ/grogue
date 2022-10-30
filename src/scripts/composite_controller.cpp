#include "composite_controller.hpp"
#include "others/data.hpp"

void CompositeController::Update() {
    auto compositePanel = GameData::Instance()->GetCompositePanel()->GetComponent<component::GridPanel>();
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_A)) {
        compositePanel->MoveHoverLeft();
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_D)) {
        compositePanel->MoveHoverRight();
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_W)) {
        compositePanel->MoveHoverUp();
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_S)) {
        compositePanel->MoveHoverDown();
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_TAB)) {
        auto controller = GameData::Instance()->GetHumanController();
        GameData::Instance()->ChangeController(controller);
        GameData::Instance()->GetCompositePanel()->SetActive(false);
        GameData::Instance()->GetCompositeDescriptionPanel()->SetActive(false);
    }
    
    changeDescriptionPanelCapacity(GameData::Instance()->GetCompositePanel()->GetComponent<component::GridPanel>());
}

void CompositeController::changeDescriptionPanelCapacity(component::GridPanel* panel) {
    auto& hoverPos = panel->GetHoverGridPos();
    int objectIdx = hoverPos.x + hoverPos.y * panel->gridNumInCol;
    auto it = ComposeConfigStorage::begin();
    std::advance(it, objectIdx);
    auto& materials = it->second.materials;
    auto compositeDescriptionPanel = GameData::Instance()->GetCompositeDescriptionPanel()->GetComponent<component::GridPanel>();
    compositeDescriptionPanel->capacity = materials.size();
}
