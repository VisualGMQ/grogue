import pygame


class Camera:
    def __init__(self):
        self.pos = pygame.math.Vector2(0, 0)

    def move(self, offset_x: int, offset_y: int):
        self.pos += pygame.math.Vector2(offset_x, offset_y)

    def move_to(self, x: int, y: int):
        self.pos = pygame.math.Vector2(x, y)

    def get_pos(self) -> pygame.math.Vector2:
        return self.pos
