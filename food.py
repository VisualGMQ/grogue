import tile
import pygame
import global_var


class FoodConstitution:
    def __init__(self,
                 vegetable_value: float,
                 meat_value: float,
                 fruit_value: float):
        sum_value = vegetable_value + meat_value + fruit_value
        self.__vegetble_percent: float = vegetable_value / sum_value
        self.__meat_percent: float = meat_value / sum_value
        self.__fruit_percent: float = fruit_value / sum_value

    def get_hp_recover(self):
        return self.__hp_recover

    def get_nutrition(self):
        return self.__nutrition

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
                 constitution: FoodConstitution,
                 nutrition: int,
                 hp_recover: int,
                 description: str):
        self.image = tile
        self.__constitution = constitution
        self.__description = description
        self.__hp_recover = hp_recover
        self.__nutrition = nutrition

    def get_constitution(self):
        return self.__constitution

    def get_nutrition(self):
        return self.__nutrition

    def get_hp_recover(self):
        return self.__hp_recover

    def get_description(self):
        return self.__description


FoodInfoTable = {
    'berry': {
        'image_idx': (0, 3),
        'constitution': FoodConstitution(10, 0, 90),
        'description': "好吃的浆果，可以回血",
        'nutrition': 10,
        'hp-recover': 2,
    },
}


def create_food(name: str):
    food = FoodInfoTable[name]
    return Food(global_var.GameTilesheet.get(food['image_idx']),
                food['constitution'],
                food['nutrition'],
                food['hp-recover'],
                food['description'])
