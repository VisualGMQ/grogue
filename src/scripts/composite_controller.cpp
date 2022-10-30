#include "composite_controller.hpp"
#include "others/data.hpp"
#include <utility>

void CompositeController::Update() {
  auto compositePanel = GameData::Instance()
                            ->GetCompositePanel()
                            ->GetComponent<component::GridPanel>();
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
  if (engine::Input::IsKeyPressed(SDL_SCANCODE_J)) {
    tryComposeObject(compositePanel);
  }

  changeDescriptionPanelCapacity(GameData::Instance()
                                     ->GetCompositePanel()
                                     ->GetComponent<component::GridPanel>());
}

void CompositeController::changeDescriptionPanelCapacity(component::GridPanel* panel) {
    // FIXME refactory
    auto& hoverPos = panel->GetHoverGridPos();
    int objectIdx = hoverPos.x + hoverPos.y * panel->gridNumInCol;
    auto it = ComposeConfigStorage::begin();
    std::advance(it, objectIdx);
    auto& materials = it->second.materials;
    auto compositeDescriptionPanel = GameData::Instance()->GetCompositeDescriptionPanel()->GetComponent<component::GridPanel>();
    compositeDescriptionPanel->capacity = materials.size();
}

bool CompositeController::tryComposeObject(component::GridPanel* panel) {
    // FIXME refactory
    auto& hoverPos = panel->GetHoverGridPos();
    int objectIdx = hoverPos.x + hoverPos.y * panel->gridNumInCol;
    auto it = ComposeConfigStorage::begin();
    std::advance(it, objectIdx);
    auto& materials = it->second.materials;

    auto backpack = GameData::Instance()->GetPlayer()->GetComponent<component::Backpack>();
    if (!backpack) return false;
    
    std::vector<std::pair<component::Pickupable*, int>> pickupables;
    for (auto& material : materials) {
        bool found = false;
        for (auto& object : backpack->objects) {
            auto pickupable = object->GetComponent<component::Pickupable>();
            if (material.id == pickupable->id && material.num <= pickupable->num) {
                pickupables.push_back(std::make_pair(pickupable, material.num));
                found = true;
                break; 
            }
        }
        if (!found) {
			return false;
        }
    }

    for (auto& pickup : pickupables) {
        pickup.first->num -= pickup.second;
        if (pickup.first->num == 0) {
            backpack->RemoveObject(pickup.first->id);
        }
    }
	// FIXME check the null pointer
	backpack->AddObject(CreatePickupable(*ObjectConfigStorage::Find(it->first)));
    return true;
}
