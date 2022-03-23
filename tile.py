import pygame
import defs


class Tile(pygame.sprite.Sprite):
    def __init__(self, tile: pygame.Surface.subsurface):
        pygame.sprite.Sprite.__init__(self)

        self.image = pygame.transform.scale(tile, (defs.TileSize, defs.TileSize))
        self.rect = tile.get_rect()

    def MoveTo(self, x: int, y: int):
        self.rect.x = x
        self.rect.y = y

    def Move(self, x: int, y: int):
        self.rect.x += x
        self.rect.y += y

    def update(self, *args, **kwargs) -> None:
        pass
