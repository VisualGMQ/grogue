#pragma once

#include "image.hpp"

namespace grogue::core {

class TileSheet final {
public:
    TileSheet(Image image, unsigned int column, unsigned int row);

    unsigned int GetRow() const { return row_; }
    unsigned int GetCol() const { return col_; }
    
    Image Get(unsigned int col, unsigned int row) const;

private:
    Image image_;
    unsigned int col_;
    unsigned int row_;
    unsigned int tileWidth_;
    unsigned int tileHeight_;
};

}
