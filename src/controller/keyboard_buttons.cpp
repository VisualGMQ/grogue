#include "keyboard_buttons.hpp"
#include "human_controller.hpp"
#include "others/error.hpp"

namespace controller::keyboard {

void MoveButton::Update() {
    if (IsTriggered()) {
        GetController()->GetVelocity() += velocity_;
    }
}

void PickupButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        component::Backpack* backpack;
        MATCH_INTO_VAR_OR_RETURN_VOID(GetController()->GetEntity()->GetComponent<component::Backpack>(), backpack);
        MonsterAction::Pickup(backpack);
    }
}

void PutButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        component::HandFrame* handFrame;
        if (type_ == HandType::Left) {
            MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetLeftHandObjectFrame().Except(DONT_EXISTS_ERR("left hand object frame"))->GetComponent<component::HandFrame>(),
                                          handFrame);
        } else {
            MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetRightHandObjectFrame().Except(DONT_EXISTS_ERR("right hand object frame"))->GetComponent<component::HandFrame>(),
                                          handFrame);
        }
        if (!handFrame->entity) return;

        component::Pickupable* pickupable;
        MATCH_INTO_VAR_OR_RETURN_VOID(handFrame->entity->GetComponent<component::Pickupable>(), pickupable);
        auto id = pickupable->id;
        auto compositeConfig = ComposeConfigStorage::Find(id);
        if (!compositeConfig) return;

        auto objectConfig = ObjectConfigStorage::Find(compositeConfig->target);

        if (objectConfig->type == ObjectConfig::Architecture) {
            auto& gridPos = GameData::Instance().GetPutTargetGridPos();

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

            if (pickupable->num - 1 > 0) {
                pickupable->num -= 1;
            } else {
                handFrame->entity = nullptr;
                component::Backpack* backpack;
                MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetPlayer().Except(DONT_EXISTS_ERR("player"))->GetComponent<component::Backpack>(), backpack);
                backpack->RemoveObject(id);
            }
        }
    }
}

void OpenBackpackButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        GameData::Instance().ChangeController(GameData::Instance().GetBackpackController());
        GameData::Instance().GetBackpackPanel().Except("backpack panel don't exists")->SetActive(true);
    }
}

void OpenCompositePanel::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        GameData::Instance().ChangeController(GameData::Instance().GetCompositeController());
        GameData::Instance().GetCompositePanel().Except("composite panel don't exists")->SetActive(true);
        GameData::Instance().GetCompositeDescriptionPanel().Except("composite description panel don't exists")->SetActive(true);
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
        GameData::Instance().GetBackpackPanel().Except("backpack panel don't exists")->SetActive(false);
        GameData::Instance().ChangeController(GameData::Instance().GetHumanController());
    }
}

void LeftHandSelectButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        component::HandFrame* handFrame;
        MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetLeftHandObjectFrame().Except(DONT_EXISTS_ERR("left hand object"))->GetComponent<component::HandFrame>(), handFrame);

        engine::Entity* object;
        MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetBackpackHoverObject(), object);

        if (object) {
            handFrame->entity = object;
        }
    }
}

void RightHandSelectButton::Update() {
    if (engine::Input::IsKeyPressed(GetKey())) {
        component::HandFrame* handFrame;
        MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetRightHandObjectFrame().Except(DONT_EXISTS_ERR("right hand object"))->GetComponent<component::HandFrame>(),
                                      handFrame);

        engine::Entity* object;
        MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetBackpackHoverObject(), object);

        if (object) {
            handFrame->entity = object;
        }
    }
}


void ComposeButton::Update() {
    if (engine::Input::IsKeyPressed(key_)) {
        component::GridPanel* panel;
        MATCH_INTO_VAR_OR_RETURN_VOID(GameData::Instance().GetCompositePanel().Except(DONT_EXISTS_ERR("composite panel"))->GetComponent<component::GridPanel>(), panel);
        tryComposeObject(panel);
    }
}

bool ComposeButton::tryComposeObject(component::GridPanel* panel) {
    auto objectIdx = panel->GetHoverIndex();
    auto it = ComposeConfigStorage::begin();
    std::advance(it, objectIdx);
    auto& materials = it->second.materials;

    component::Backpack* backpack;
    MATCH_INTO_VAR_OR_RETURN(GameData::Instance().GetPlayer().Except(DONT_EXISTS_ERR("player"))->GetComponent<component::Backpack>(), backpack, false);
    
    std::vector<std::pair<component::Pickupable*, int>> pickupables;
    for (auto& material : materials) {
        bool found = false;
        for (auto& object : backpack->objects) {
            component::Pickupable* pickupable;
            MATCH_INTO_VAR_OR(object->GetComponent<component::Pickupable>(), pickupable, continue;);

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
      auto controller = GameData::Instance().GetHumanController();
      GameData::Instance().ChangeController(controller);
      GameData::Instance().GetCompositePanel().Except("composite panel don't exists")->SetActive(false);
      GameData::Instance().GetCompositeDescriptionPanel().Except("composite description panel don't exists")->SetActive(false);
  }
}

}