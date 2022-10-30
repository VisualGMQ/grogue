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
    position.Set(-1, -1);
    showCursor = true;
}

void GridPanel::MoveHoverLeft() {
    if (isInRange(hoverGridPos_.x - 1, hoverGridPos_.y) &&
        hoverGridPos_.x - 1 >= 0) {
        hoverGridPos_.x --;
        tryCallHoverMove();
    }
}

void GridPanel::MoveHoverRight() {
    if (isInRange(hoverGridPos_.x + 1, hoverGridPos_.y) &&
        hoverGridPos_.x + 1 < gridNumInCol) {
        hoverGridPos_.x ++;
        tryCallHoverMove();
    }
}

void GridPanel::MoveHoverUp() {
    if (isInRange(hoverGridPos_.x, hoverGridPos_.y - 1) &&
        hoverGridPos_.y - 1 >= 0) {
        hoverGridPos_.y --;
        tryCallHoverMove();
    }
}

void GridPanel::MoveHoverDown() {
    int row = std::ceil(capacity / (float)gridNumInCol);
    if (isInRange(hoverGridPos_.x, hoverGridPos_.y + 1) &&
        hoverGridPos_.y + 1 < row) {
        hoverGridPos_.y ++;
        tryCallHoverMove();
    }
}

bool GridPanel::isInRange(int x, int y) {
    int idx = x + y * gridNumInCol;
    return idx >= 0 && idx < capacity;
}

}
