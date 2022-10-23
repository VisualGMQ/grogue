#include "backpack_panel.hpp"

namespace component {

void BackpackPanel::Reset() {
    capacity = 20;
    gridSize = 40;
    gridPadding = 10;
    gridNumInCol = 10;
    hoverGridPos_.Set(0, 0);
}

void BackpackPanel::MoveHoverLeft() {
    hoverGridPos_.x = std::max(0.0f, hoverGridPos_.x - 1);
}

void BackpackPanel::MoveHoverRight() {
    hoverGridPos_.x = std::min<float>(gridNumInCol - 1, hoverGridPos_.x + 1);
}

void BackpackPanel::MoveHoverUp() {
    hoverGridPos_.y = std::max<float>(0, hoverGridPos_.y - 1);
}

void BackpackPanel::MoveHoverDown() {
    int row = capacity / gridNumInCol;
    hoverGridPos_.y = std::min<float>(row - 1, hoverGridPos_.y + 1);
}

}