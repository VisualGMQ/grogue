import pygame


class Circle:
    def __init__(self, center: pygame.math.Vector2, r: float):
        self.center = center
        self.r = r

    def __str__(self):
        return "Circle(" + self.center + "," + self.r + ")"


def is_circle_collide(c1: Circle, c2: Circle) -> bool:
    return (c1.center - c2.center).length() < c1.r + c2.r


def is_point_in_circle(p: pygame.math.Vector2, c: Circle) -> bool:
    return (c.center - p).length() < c.r
