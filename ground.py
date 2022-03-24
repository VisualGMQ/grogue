import tile
import pygame
import enum


class GroundType(enum.IntEnum):
    Dirt = 0
    Stone = 1


class Ground(tile.Tile):
    def __init__(self, image: pygame.Surface.subsurface, type: GroundType):
        tile.Tile.__init__(self, image)
        self.__type = type

    def get_type(self) -> GroundType:
        return self.__type


GroundInfo = {
    GroundType.Dirt: (2, 1),
}
