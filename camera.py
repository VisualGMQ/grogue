import pygame
import defs


class Camera:
    def __init__(self):
        self.__pos = pygame.math.Vector2(0, 0)

    def move(self, offset_x: int, offset_y: int):
        self.__pos += pygame.math.Vector2(offset_x, offset_y)

    def move_to(self, x: int, y: int):
        self.__pos = pygame.math.Vector2(x, y)

    def get_pos(self) -> pygame.math.Vector2:
        return self.__pos

    def stare_at(self, x: int, y: int, map_w: int, map_h: int):
        screen_tile_x = defs.ScreenWidth // defs.TileSize
        screen_tile_y = defs.ScreenHeight // defs.TileSize

        self.move_to((x - screen_tile_x // 2) * defs.TileSize,
                     (y - screen_tile_y // 2) * defs.TileSize)

        if self.__pos.x < 0:
            self.__pos.x = 0
        if self.__pos.y < 0:
            self.__pos.y = 0
        if self.__pos.x + defs.ScreenWidth >= map_w * defs.TileSize:
            self.__pos.x = max(map_w * defs.TileSize, defs.ScreenWidth) - defs.ScreenWidth
        if self.__pos.y + defs.ScreenHeight >= map_h * defs.TileSize:
            self.__pos.y = max(map_h * defs.TileSize, defs.ScreenHeight) - defs.ScreenHeight
