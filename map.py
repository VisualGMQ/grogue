import tile
import geometry
import random
import pygame
import ground
import global_var
import defs
import plant

class GameMap:
    def __init__(self, w: int, h: int):
        self.__w = w
        self.__h = h
        self.__data = []

        for i in range(self.__w * self.__h):
            self.__data.append(None)

    def get(self, x: int, y: int) -> tile.Tile:
        return self.__data[x + y * self.__w]

    def set(self, x: int, y: int, entity: tile.Tile):
        self.__data[x + y * self.__w] = entity

    def get_width(self) -> int:
        return self.__w

    def get_height(self) -> int:
        return self.__h

    def contain(self, x: int, y: int) -> bool:
        return x >= 0 and y >= 0 and x < self.__w and y < self.__h


def map_generate(w: int, h: int, r: int, try_count: int) -> (GameMap, pygame.math.Vector2, list):
    circle_list = []
    path_list = []
    building_list = []
    for i in range(try_count):
        circle = geometry.Circle(pygame.math.Vector2(random.randint(r, w - r),
                                                     random.randint(r, h - r)),
                                 r)
        if len(circle_list) == 0:
            circle_list.append(circle)
        else:
            can_put = True
            for c in circle_list:
                if geometry.is_circle_collide(c, circle):
                    can_put = False
                    break
            if can_put:
                circle_list.append(circle)

                dir = circle_list[-1].center - circle_list[-2].center
                path_r = r
                step = dir.length() / path_r
                dir = dir.normalize() * path_r
                for i in range(0, int(step)):
                    path_list.append(geometry.Circle(i * dir + circle_list[-2].center,
                                                     random.randint(1, r)))

    game_map = GameMap(w, h)

    for circle in circle_list:
        for x in range(int(circle.center.x - circle.r), int(circle.center.x + circle.r)):
            for y in range(int(circle.center.y - circle.r), int(circle.center.y + circle.r)):
                if geometry.is_point_in_circle((x, y), circle):
                    g = ground.Ground(global_var.GameTilesheet.get(ground.GroundInfo[ground.GroundType.Dirt]),
                                      ground.GroundType.Dirt)
                    g.move_to(x, y)
                    game_map.set(x, y, g)
                    if random.randint(1, 100) < 30:
                        berryClusterInfo = plant.PlantInfoTable['berry cluster']
                        p = plant.Plant(berryClusterInfo['threshold'],
                                        global_var.GameTilesheet.get(berryClusterInfo['tile_pos']),
                                        global_var.GameTilesheet.get(berryClusterInfo['growth_tile_pos']))
                        p.grow(random.randint(1, 110))
                        p.move_to(x, y)
                        building_list.append(p)

    for circle in path_list:
        for x in range(int(circle.center.x - circle.r), int(circle.center.x + circle.r)):
            for y in range(int(circle.center.y - circle.r), int(circle.center.y + circle.r)):
                if geometry.is_point_in_circle((x, y), circle) and game_map.contain(x, y):
                    g = ground.Ground(global_var.GameTilesheet.get(ground.GroundInfo[ground.GroundType.Dirt]),
                                      ground.GroundType.Dirt)
                    g.move_to(x, y)
                    game_map.set(x, y, g)

    return (game_map, circle_list[random.randint(0, len(circle_list)) - 1].center, building_list)
