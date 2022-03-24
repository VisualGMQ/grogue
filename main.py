import game_template
import tilesheet
import pygame
from pygame.locals import *
import enum
import tile
import plant
import creature
import controller
import camera
import global_var
import map
import defs
import collide


class LayerEnum(enum.IntEnum):
    Background = 0
    Item = 1
    Building = 2
    Creature = 3

    LayerNum = 4


class Game(game_template.GameTemplate):
    def __init__(self):
        game_template.GameTemplate.__init__(self)

    def on_init(self):
        game_template.GameTemplate.on_init(self)

        self.__layers = []
        for i in range(0, LayerEnum.LayerNum):
            self.__layers.append(pygame.sprite.Group())

        global_var.GameTilesheet = tilesheet.Tilesheet(filename='assets/tilesheet.png',
                                                       w=8, h=8)

        (self.__map, player_init_pos) = map.map_generate(10 * self._window.get_width() // defs.TileSize,
                                                         10 * self._window.get_height() // defs.TileSize,
                                                         4,
                                                         1000)
        for x in range(self.__map.get_width()):
            for y in range(self.__map.get_height()):
                if self.__map.get(x, y) is not None:
                    self.__layers[LayerEnum.Background].add(self.__map.get(x, y))

        self.__player = creature.Creature(image=global_var.GameTilesheet.get((0, 0)))
        self.__player.move(player_init_pos.x, player_init_pos.y)
        self.__layers[LayerEnum.Creature].add(self.__player)
        self.__controller = controller.Controller(self.__player)
        self.__camera = camera.Camera()

    def on_update(self, elapse: int):
        self.__controller.update()

        for i in range(len(self.__layers)):
            self.__layers[i].update()

        for c in self.__layers[LayerEnum.Creature]:
            collide.out_of_map_range_fix(self.__map, c)

        self.__camera.stare_at(self.__player.pos.x, self.__player.pos.y,
                               self.__map.get_width(), self.__map.get_height())

    def on_render(self):
        for i in range(len(self.__layers)):
            for sprite in self.__layers[i]:
                blit_pos = (sprite.rect.x - self.__camera.get_pos().x,
                            sprite.rect.y - self.__camera.get_pos().y)
                if pygame.Rect(0, 0,
                               self._window.get_width(),
                               self._window.get_height()).colliderect(pygame.Rect(blit_pos[0], blit_pos[1], defs.TileSize, defs.TileSize)):
                    self._window.blit(sprite.image, blit_pos)


if __name__ == '__main__':
    theGame = Game()
    theGame.on_run()
