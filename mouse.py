import pygame
import copy


class MouseInput:
    def __init__(self):
        self.__button_status = [False, False, False, False, False]
        self.__old_button_status = copy.deepcopy(self.__button_status)

    def update_old_status(self):
        self.__old_button_status = copy.deepcopy(self.__button_status)

    def update(self, is_press: bool, index: int):
        self.__button_status[index] = is_press

    def get_pos(self) -> pygame.math.Vector2:
        return pygame.math.Vector2(pygame.mouse.get_pos())

    def is_left_pressed(self) -> bool:
        return not self.__old_button_status[0] and self.__button_status[0]

    def is_right_pressed(self) -> bool:
        return not self.__old_button_status[2] and self.__button_status[2]


Mouse: MouseInput = MouseInput()
