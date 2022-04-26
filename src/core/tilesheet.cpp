#include "core/tilesheet.hpp"

namespace grogue::core {

TileSheet::TileSheet(Image image, unsigned int column, unsigned int row)
    : image_(image),
      col_(column), row_(row),
      tileWidth_(image.GetSize().w / column), tileHeight_(image.GetSize().h / row) {}

Image TileSheet::Get(unsigned int col, unsigned int row) const {
    return Image::Create(image_, Recti(col * tileWidth_, row * tileHeight_, tileWidth_, tileHeight_)); 
}

}
