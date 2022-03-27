import tile
import defs
import pygame


class Creature(tile.Tile):
    def __init__(self, image: pygame.Surface.subsurface):
        tile.Tile.__init__(self, image)
        self.__direction = 1
        self.__image = pygame.transform.scale(image,
                                              (defs.TileSize, defs.TileSize))
        self.old_pos = pygame.math.Vector2(0, 0)

        self.__hp = 100
        self.__max_hp = 100
        self.__nutrition = 1000   # 1000 for full health

        self.__items = []

    def add_item(self, item):
        self.__items.append(item)

    def get_items(self):
        return self.__items

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

    def set_nutrition(self, value: int):
        self.__nutrition = max(0, min(value, 1000))

    def set_hp(self, value: int):
        self.__hp = min(max(value, 0), self.__max_hp)

    def get_hp(self):
        return self.__hp

    def set_max_hp(self, value: int):
        self.__max_hp = value

    def get_max_hp(self):
        return self.__max_hp

    def get_nutrition(self):
        return self.__nutrition

    def is_nutrition_full(self):
        return self.__nutrition >= 1000

    def is_nutrition_normal(self):
        return self.__nutrition < 1000 and self.__nutrition > 300

    def is_nutrition_hungry(self):
        return self.__nutrition > 100 and self.__nutrition <= 300

    def is_nutrition_starve(self):
        return self.__nutrition >= 0 and self.__nutrition <= 100


def get_nutrition_str(creature: Creature) -> (str, tuple):
    if creature.is_nutrition_full():
        return ('饱腹',   (0, 255, 0, 255))
    elif creature.is_nutrition_hungry():
        return ('饥饿', (255, 112, 0, 255))
    elif creature.is_nutrition_normal():
        return ('正常', (125, 125, 125, 255))
    elif creature.is_nutrition_starve():
        return ('极饿', (255, 0, 0, 255))
