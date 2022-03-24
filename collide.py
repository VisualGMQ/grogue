import pygame
import map
import creature


def out_of_map_range_fix(m: map.GameMap, c: creature.Creature):
    if m.get(int(c.pos.x), int(c.pos.y)) is None or not m.contain(int(c.pos.x), int(c.pos.y)):
        c.back_to_oldpos()
