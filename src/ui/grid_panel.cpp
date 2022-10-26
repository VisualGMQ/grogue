#include "grid_panel.hpp"

namespace component {

void GridPanel::Reset() {
    capacity = 20;
    gridSize = 40;
    gridPadding = 10;
    gridNumInCol = 10;
    hoverGridPos_.Set(0, 0);
    onHoverMove = nullptr;
    drawImageFunc = nullptr;
}

void GridPanel::MoveHoverLeft() {
    if (hoverGridPos_.x > 0) {
        hoverGridPos_.x --;
        tryCallHoverMove();
    }
}

void GridPanel::MoveHoverRight() {
    if (hoverGridPos_.x < gridNumInCol - 1) {
        hoverGridPos_.x ++;
        tryCallHoverMove();
    }
}

void GridPanel::MoveHoverUp() {
    if (hoverGridPos_.y > 0 ) {
        hoverGridPos_.y --;
        tryCallHoverMove();
    }
}

void GridPanel::MoveHoverDown() {
    int row = capacity / gridNumInCol;
    if (hoverGridPos_.y < row - 1) {
        hoverGridPos_.y ++;
        tryCallHoverMove();
    }
}

}