import creature
import pygame
from pygame.locals import *
import key
import global_var


class Controller:
    def __init__(self, creature: creature.Creature):
        self.creature = creature

    def update(self):
        if not global_var.ShowItems:
            if key.Key.is_pressed(K_a):
                self.creature.move_left()
            if key.Key.is_pressed(K_d):
                self.creature.move_right()
            if key.Key.is_pressed(K_w):
                self.creature.move_up()
            if key.Key.is_pressed(K_s):
                self.creature.move_down()
        if key.Key.is_pressed(K_TAB):
            global_var.toggle_show_items()
