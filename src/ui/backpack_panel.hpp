#pragma once

#include "engine/engine.hpp"

namespace component {

class BackpackPanel: public engine::Component {
public:
    BackpackPanel(engine::ComponentID id): engine::Component(id) {
        Reset();
        if (!engine::ImageFactory::Find("tilesheet#select_outline", selectOutline)) {
            Loge("select_outline image not in tilesheet");
        }
    }
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
    engine::Image selectOutline;

private:
    engine::Vec2 hoverGridPos_;
};

}