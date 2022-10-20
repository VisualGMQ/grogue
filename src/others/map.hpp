#include "engine/engine.hpp"
#include "components/block.hpp"

class Map final {
public:
    struct Tile {
        engine::Entity* terrian = nullptr;
        engine::Entity* object = nullptr;
        bool canSee = false;
        bool visited = false;
    };

    Map(int w, int h): size_(w, h) {
        tiles_.resize(w * h);
    }
    Tile& Get(int x, int y) { return tiles_[x * size_.w + y]; }
    const engine::Size& GetSize() const { return size_; }

private:
    std::vector<Tile> tiles_;
    engine::Size size_;
};