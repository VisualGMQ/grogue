import enum

TileSize = 32

ScreenWidth = 1024
ScreenHeight = 720


class LayerEnum(enum.IntEnum):
    Background = 0
    Item = 1
    Building = 2
    Creature = 3

    LayerNum = 4
