import pygame
from pygame.locals import *


class Tilesheet:
    def __init__(self, filename: str, w: int, h: int):
        self.image = pygame.image.load(filename)
        self.tile_w = self.image.get_width() / w
        self.tile_h = self.image.get_height() / h

    def get(self, idx) -> pygame.Surface.subsurface:
        if type(idx) == int:
            return self.image.subsurface(
                    pygame.Rect(self.tile_w * (idx % self.tile_w),
                                self.tile_h * (idx // self.tile_h),
                                self.tile_w, self.tile_h))
        elif type(idx) == tuple:
            x = idx[0]
            y = idx[1]
            return self.image.subsurface(
                    pygame.Rect(x * self.tile_w, y * self.tile_h,
                                self.tile_w, self.tile_h))
        else:
            assert False, "[Tilesheet::get]: Invalid function call"
