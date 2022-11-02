#pragma once

#include "engine/engine.hpp"
#include "button.hpp"
#include "others/monster_action.hpp"
#include "components/human.hpp"
#include "components/rigidbody.hpp"
#include "components/sprite.hpp"
#include "components/backpack.hpp"
#include "ui/hand_frame.hpp"
#include "ui/grid_panel.hpp"
#include "others/compose_reader.hpp"
#include "others/object_reader.hpp"

class HumanController;
class BackpackController;

namespace controller::keyboard {

class KeyboardButton: public Button {
public:
    KeyboardButton(SDL_Scancode key): key_(key) {}
    SDL_Scancode GetKey() const { return key_; }

private:
    SDL_Scancode key_;
};

class HumanActionButton: public KeyboardButton {
public:
    HumanActionButton(::HumanController* controller, SDL_Scancode key): KeyboardButton(key), controller_(controller) {}
    ::HumanController* GetController() { return controller_; }

private:
    ::HumanController* controller_;
};

class MoveButton: public HumanActionButton {
public:
    MoveButton(::HumanController* controller, const engine::Vec2& velocity, SDL_Scancode key): HumanActionButton(controller, key), velocity_(velocity) {}
    bool IsTriggered() const { return engine::Input::IsKeyPressing(GetKey()); }
    void Update() override;

private: 
    engine::Vec2 velocity_;
};

class PickupButton: public HumanActionButton{
public:
    PickupButton(::HumanController* controller, SDL_Scancode key): HumanActionButton(controller, key) {}
    void Update() override;
};

class OpenBackpackButton: public HumanActionButton {
public:
    OpenBackpackButton(::HumanController* controller, SDL_Scancode key): HumanActionButton(controller, key) {}
    void Update() override;
};

class OpenCompositePanel: public HumanActionButton {
public:
    OpenCompositePanel(::HumanController* controller, SDL_Scancode key): HumanActionButton(controller, key) {}
    void Update() override;
};

class GridPanelMoveButton: public KeyboardButton {
public:
    GridPanelMoveButton(component::GridPanel* panel, SDL_Scancode key): KeyboardButton(key), panel_(panel) {}
    auto GetPanel() { return panel_; }

private:
    component::GridPanel* panel_;
};

class GridPanelMoveLeftButton: public GridPanelMoveButton {
public:
    GridPanelMoveLeftButton(component::GridPanel* panel, SDL_Scancode key): GridPanelMoveButton(panel, key) {}

    void Update() override;
};

class GridPanelMoveRightButton: public GridPanelMoveButton {
public:
    GridPanelMoveRightButton(component::GridPanel* panel, SDL_Scancode key): GridPanelMoveButton(panel, key) {}
    void Update() override;
};

class GridPanelMoveDownButton: public GridPanelMoveButton {
public:
    GridPanelMoveDownButton(component::GridPanel* panel, SDL_Scancode key): GridPanelMoveButton(panel, key) {}
    void Update() override;
};

class GridPanelMoveUpButton: public GridPanelMoveButton {
public:
    GridPanelMoveUpButton(component::GridPanel* panel, SDL_Scancode key): GridPanelMoveButton(panel, key) {}
    void Update() override;
};

class CloseBackpackButton: public KeyboardButton {
public:
    CloseBackpackButton(SDL_Scancode key): KeyboardButton(key){}
    void Update() override;
}; 

class LeftHandSelectButton: public KeyboardButton {
public:
    LeftHandSelectButton(SDL_Scancode key): KeyboardButton(key) {}
    void Update() override;
};

class RightHandSelectButton: public KeyboardButton {
public:
    RightHandSelectButton(SDL_Scancode key): KeyboardButton(key) {}
    void Update() override;
};

class ComposeButton: public Button {
public:
    ComposeButton(SDL_Scancode key): key_(key) {}
    void Update() override;

private:
    SDL_Scancode key_;

    bool tryComposeObject(component::GridPanel* panel);
};

class CloseCompositePanelButton: public Button {
public:
    CloseCompositePanelButton(SDL_Scancode key): key_(key) {}
    void Update() override;

private:
    SDL_Scancode key_;
};



}