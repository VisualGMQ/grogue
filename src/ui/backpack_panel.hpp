#pragma once

#include "engine/engine.hpp"

namespace component {

class BackpackPanel: public engine::Component {
public:
    BackpackPanel(engine::ComponentID id): engine::Component(id) { Reset(); }
    void Reset() override;
    

    void MoveHoverRight();
    void MoveHoverDown();
    void MoveHoverUp();
    void MoveHoverLeft();
    const engine::Vec2& GetHoverGridPos() const { return hoverGridPos_; }

    int capacity;
    int gridSize;
    int gridPadding;
    int gridNumInCol;

private:
    engine::Vec2 hoverGridPos_;
};

}