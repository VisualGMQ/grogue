import game_template
import tilesheet
import pygame
from pygame.locals import *
import enum
import tile
import plant
import creature
import controller

GameTilesheet: tilesheet.Tilesheet = None


class LayerEnum(enum.IntEnum):
    Background = 0
    Item = 1
    Building = 2
    Creature = 3

    LayerNum = 4


class Game(game_template.GameTemplate):
    def __init__(self):
        self.__layers = []
        for i in range(0, LayerEnum.LayerNum):
            self.__layers.append(pygame.sprite.LayeredUpdates(default_layer=i))

        global GameTilesheet
        GameTilesheet = tilesheet.Tilesheet(filename='assets/tilesheet.png',
                                            w=8, h=8)
        self.__layers[LayerEnum.Background].add(tile.Tile(GameTilesheet.get((2, 1))))
        self.__layers[LayerEnum.Building].add(plant.Plant(grown_threshold=10,
                                                          groth_tile=GameTilesheet.get((0, 2)),
                                                          grown_tile=GameTilesheet.get((1, 2))))
        self.__player = creature.Creature(image=GameTilesheet.get((0, 0)))
        self.__layers[LayerEnum.Creature].add(self.__player)
        self.__controller = controller.Controller(self.__player)

    def on_update(self, elapse: int):
        for i in range(len(self.__layers)):
            self.__layers[i].update()
        self.__controller.update()

    def on_render(self):
        for i in range(len(self.__layers)):
            self.__layers[i].draw(self._window)


if __name__ == '__main__':
    theGame = Game()
    theGame.on_run()
