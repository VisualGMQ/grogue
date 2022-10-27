#include "backpack_controller.hpp"
#include "others/data.hpp"

void BackpackController::Update() {
    auto backpackPanel = GameData::Instance()->GetBackpackPanel()->GetComponent<component::GridPanel>();
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_A)) {
        backpackPanel->MoveHoverLeft();
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_D)) {
        backpackPanel->MoveHoverRight();
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_W)) {
        backpackPanel->MoveHoverUp();
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_S)) {
        backpackPanel->MoveHoverDown();
    }
    if (engine::Input::IsKeyPressed(SDL_SCANCODE_E)) {
        GameData::Instance()->GetBackpackPanel()->SetActive(false);
        GameData::Instance()->ChangeController(GameData::Instance()->GetHumanController());
    }
}