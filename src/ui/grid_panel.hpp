#pragma once

#include "engine/engine.hpp"

namespace component {

class GridPanel: public engine::Component {
public:
    GridPanel(engine::ComponentID id): engine::Component(id) { Reset(); }
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
    engine::Vec2 position;
    bool showCursor;

    using HoverMoveCb = std::function<void(GridPanel*)>;
    HoverMoveCb onHoverMove;

    using DrawImageFunc = std::function<void(GridPanel*, const engine::Rect&, int, int)>;
    DrawImageFunc drawImageFunc;

private:
    engine::Vec2 hoverGridPos_;

    void tryCallHoverMove() {
        if (onHoverMove) onHoverMove(this);
    }

    bool isInRange(int x, int y);
};

}
