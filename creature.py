import tile
import defs
import pygame


class Creature(tile.Tile):
    def __init__(self, image: pygame.Surface.subsurface):
        tile.Tile.__init__(self, image)
        self.__direction = 1
        self.__image = pygame.transform.scale(image, (defs.TileSize, defs.TileSize))
        self.old_pos = pygame.math.Vector2(0, 0)

    def move_left(self):
        self.__record_oldpos()
        self.pos.x -= 1
        self.__update_rect()
        self.__change_direction(-1)

    def move_right(self):
        self.__record_oldpos()
        self.pos.x += 1
        self.__update_rect()
        self.__change_direction(1)

    def move_up(self):
        self.__record_oldpos()
        self.pos.y -= 1
        self.__update_rect()

    def move_down(self):
        self.__record_oldpos()
        self.pos.y += 1
        self.__update_rect()

    def back_to_oldpos(self):
        self.pos = self.old_pos
        self.__update_rect()

    def __record_oldpos(self):
        self.old_pos = self.pos.copy()

    def __update_rect(self):
        self.rect.x = self.pos.x * defs.TileSize
        self.rect.y = self.pos.y * defs.TileSize

    def __change_direction(self, dir: int):
        self.__direction = dir
        if dir < 0:
            self.image = pygame.transform.flip(self.__image, True, False)
        else:
            self.image = self.__image
