import tile
import defs
import pygame


class Creature(tile.Tile):
    def __init__(self, image: pygame.Surface.subsurface):
        tile.Tile.__init__(self, image)
        self.__direction = 1
        self.__image = pygame.transform.scale(image, (defs.TileSize, defs.TileSize))

    def MoveLeft(self):
        self.rect.x -= defs.TileSize
        self.__changeDirection(-1)

    def MoveRight(self):
        self.rect.x += defs.TileSize
        self.__changeDirection(1)

    def MoveUp(self):
        self.rect.y -= defs.TileSize

    def MoveDown(self):
        self.rect.y += defs.TileSize

    def __changeDirection(self, dir: int):
        self.__direction = dir
        if dir < 0:
            self.image = pygame.transform.flip(self.__image, True, False)
        else:
            self.image = self.__image
