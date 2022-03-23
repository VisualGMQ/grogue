import tile
import pygame


class FoodConstitution:
    def __init__(self,
                 vegetable_value: float,
                 meat_value: float,
                 fruit_value: float):
        sum_value = vegetable_value + meat_value + fruit_value
        self.__vegetble_percent: float = vegetable_value / sum_value
        self.__meat_percent: float = meat_value / sum_value
        self.__fruit_percent: float = fruit_value / sum_value

    def get_vegetable_percent(self):
        return self.__vegetble_percent

    def get_meat_percent(self):
        return self.__meat_percent

    def get_fruit_percent(self):
        return self.__fruit_percent

    def is_vegetable(self):
        return self.__vegetble_percent >= 80

    def is_meat(self):
        return self.__meat_percent >= 80

    def is_fruit(self):
        return self.__fruit_percent >= 80


class Food(tile.Tile):
    def __init__(self, tile: pygame.Surface.subsurface,
                 constitution: FoodConstitution):
        self.image = tile
        self.constitution = constitution
