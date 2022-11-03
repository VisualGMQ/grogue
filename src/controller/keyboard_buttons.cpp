#include "keyboard_buttons.hpp"
#include "human_controller.hpp"

namespace controller::keyboard {

void MoveButton::Update() {
    if (IsTriggered()) {
        GetController()->GetVelocity() += velocity_;
    }
}

void PickupButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        MonsterAction::Pickup(GetController()->GetEntity()->GetComponent<component::Backpack>());
    }
}

void PutButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        component::HandFrame* handFrame;
        if (type_ == HandType::Left) {
            handFrame = GameData::Instance()->GetLeftHandObjectFrame()->GetComponent<component::HandFrame>();
        } else {
            handFrame = GameData::Instance()->GetRightHandObjectFrame()->GetComponent<component::HandFrame>();;
        }
        if (!handFrame->entity) return;

        auto id = handFrame->entity->GetComponent<component::Pickupable>()->id;
        auto compositeConfig = ComposeConfigStorage::Find(id);
        if (!compositeConfig) return;

        auto objectConfig = ObjectConfigStorage::Find(compositeConfig->target);

        if (objectConfig->type == ObjectConfig::Architecture) {
            auto& gridPos = GameData::Instance()->GetPutTargetGridPos();

            auto map = MapManager::GetGroundMap();
            auto mapSize = map->GetSize();
            if (gridPos.x < 0 || gridPos.x >= mapSize.w || gridPos.y < 0 || gridPos.y >= mapSize.h) {
                return;
            }

            auto& mapObject = map->Get(gridPos.x, gridPos.y);
            if (mapObject.object || mapObject.terrian.type == Terrian::Liquid) {
                return;
            }

            mapObject.object = CreateArchitecture(*objectConfig);
            map->UpdateArchImage(gridPos.x, gridPos.y);

            auto pickupable = handFrame->entity->GetComponent<component::Pickupable>();
            if (pickupable->num - 1 > 0) {
                pickupable->num -= 1;
            } else {
                handFrame->entity = nullptr;
                auto backpack = GameData::Instance()->GetPlayer()->GetComponent<component::Backpack>();
                backpack->RemoveObject(id);
            }
        }
    }
}

void OpenBackpackButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        GameData::Instance()->ChangeController(GameData::Instance()->GetBackpackController());
        GameData::Instance()->GetBackpackPanel()->SetActive(true);
    }
}

void OpenCompositePanel::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        GameData::Instance()->ChangeController(GameData::Instance()->GetCompositeController());
        GameData::Instance()->GetCompositePanel()->SetActive(true);
        GameData::Instance()->GetCompositeDescriptionPanel()->SetActive(true);
    }
}

void GridPanelMoveLeftButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        GetPanel()->MoveHoverLeft();
    }
}

void GridPanelMoveRightButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        GetPanel()->MoveHoverRight();
    }
}

void GridPanelMoveDownButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        GetPanel()->MoveHoverDown();
    }
}

void GridPanelMoveUpButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        GetPanel()->MoveHoverUp();
    }
}

void CloseBackpackButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        GameData::Instance()->GetBackpackPanel()->SetActive(false);
        GameData::Instance()->ChangeController(GameData::Instance()->GetHumanController());
    }
}

void LeftHandSelectButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        auto handFrame = GameData::Instance()->GetLeftHandObjectFrame()->GetComponent<component::HandFrame>();
        auto object = GameData::Instance()->GetBackpackHoverObject();
        if (object) {
            handFrame->entity = object;
        }
    }
}

void RightHandSelectButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        auto handFrame = GameData::Instance()->GetRightHandObjectFrame()->GetComponent<component::HandFrame>();
        auto object = GameData::Instance()->GetBackpackHoverObject();
        if (object) {
            handFrame->entity = object;
        }
    }
}


void ComposeButton::Update() {
    if (engine::Input::IsKeyPressed(key_)) {
        tryComposeObject(GameData::Instance()->GetCompositePanel()->GetComponent<component::GridPanel>());
    }
}

bool ComposeButton::tryComposeObject(component::GridPanel* panel) {
    auto objectIdx = panel->GetHoverIndex();
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

void CloseCompositePanelButton::Update() {
  if (engine::Input::IsKeyPressed(key_)) {
      auto controller = GameData::Instance()->GetHumanController();
      GameData::Instance()->ChangeController(controller);
      GameData::Instance()->GetCompositePanel()->SetActive(false);
      GameData::Instance()->GetCompositeDescriptionPanel()->SetActive(false);
  }
}

}