import pygame
import copy


class Keyboard:
    def __init__(self):
        self.keys: dict = {}
        self.oldkeys: dict = {}

    def update(self, is_key_down: bool, keycode: int):
        self.keys[keycode] = is_key_down

    def update_old_keys(self):
        self.oldkeys = copy.deepcopy(self.keys)

    def is_pressed(self, keycode: int) -> bool:
        if keycode in self.keys and keycode not in self.oldkeys:
            return self.keys[keycode]
        elif keycode in self.keys and keycode in self.oldkeys:
            return self.keys[keycode] and not self.oldkeys[keycode]
        else:
            return False


Key: Keyboard = Keyboard()
