import pygame
import defs


class Tile(pygame.sprite.Sprite):
    def __init__(self, tile: pygame.Surface.subsurface):
        pygame.sprite.Sprite.__init__(self)

        self.image = pygame.transform.scale(tile, (defs.TileSize, defs.TileSize))
        self.rect = tile.get_rect()
        self.pos = pygame.math.Vector2(0, 0)

    def move_to(self, x: int, y: int):
        self.pos.x = x
        self.pos.y = y

        self.rect.x = self.pos.x * defs.TileSize
        self.rect.y = self.pos.y * defs.TileSize

    def move(self, x: int, y: int):
        self.pos.x += x
        self.pos.y += y

        self.rect.x = self.pos.x * defs.TileSize
        self.rect.y = self.pos.y * defs.TileSize

    def update(self, *args, **kwargs) -> None:
        pass
