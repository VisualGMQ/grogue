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
import ui


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

        print("use font: simhei")
        global_var.Font = pygame.font.SysFont('simhei', 32) 

        self.__layers = []
        for i in range(0, LayerEnum.LayerNum):
            self.__layers.append(pygame.sprite.Group())

        global_var.GameTilesheet = tilesheet.Tilesheet(filename='assets/tilesheet.png',
                                                       w=8, h=8)

        (self.__map, player_init_pos, buildings) = map.map_generate(2 * self._window.get_width() // defs.TileSize,
                                                                    2 * self._window.get_height() // defs.TileSize,
                                                         4,
                                                         1000)
        for x in range(self.__map.get_width()):
            for y in range(self.__map.get_height()):
                if self.__map.get(x, y) is not None:
                    self.__layers[LayerEnum.Background].add(self.__map.get(x, y))

        for building in buildings:
            self.__layers[LayerEnum.Building].add(building)

        self.__player = creature.Creature(image=global_var.GameTilesheet.get((0, 0)))
        self.__player.move(player_init_pos.x, player_init_pos.y)
        self.__layers[LayerEnum.Creature].add(self.__player)
        self.__controller = controller.Controller(self.__player)
        self.__camera = camera.Camera()

        for i in range(1, 30):
            self.__player.add_item(ui.ItemDisplayInfo(global_var.GameTilesheet.get((2, 2)),
                                   "好吃的浆果，能回复饱食度"))

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

        self.__draw_hud()

    def __draw_hud(self):
        hp_text_surf = global_var.Font.render("生命值: " + str(self.__player.get_hp()),
                                              True,
                                              (200, 0, 0, 255))
        self._window.blit(hp_text_surf, (0, defs.ScreenHeight - defs.TileSize))

        nutrition_display_info = creature.get_nutrition_str(self.__player)
        nutrition_text_surf = global_var.Font.render(nutrition_display_info[0],
                                                     True,
                                                     nutrition_display_info[1])
        self._window.blit(nutrition_text_surf,
                          (200, defs.ScreenHeight - defs.TileSize))

        if global_var.ShowItems:
            ui.ItemDisplayPanel(self._window,
                                pygame.Rect(100, 10,
                                            defs.ScreenWidth - 200, 200),
                                defs.TileSize,
                                self.__player.get_items())


if __name__ == '__main__':
    theGame = Game()
    theGame.on_run()
