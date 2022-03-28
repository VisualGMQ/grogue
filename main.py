import game_template
import tilesheet
import pygame
from pygame.locals import *
import creature
import controller
import camera
import global_var
import map
import defs
import collide
import ui
import food


class Game(game_template.GameTemplate):
    def __init__(self):
        game_template.GameTemplate.__init__(self)

    def on_init(self):
        game_template.GameTemplate.on_init(self)

        print("use font: simhei")
        global_var.Font = pygame.font.SysFont('simhei', 32)

        for i in range(0, defs.LayerEnum.LayerNum):
            global_var.GameMapLayers.append(pygame.sprite.Group())

        global_var.GameTilesheet = tilesheet.Tilesheet(filename='assets/tilesheet.png',
                                                       w=8, h=8)

        (self.__map, player_init_pos, buildings) = map.map_generate(2 * self._window.get_width() // defs.TileSize,
                                                                    2 * self._window.get_height() // defs.TileSize,
                                                                    4,
                                                                    1000)
        for x in range(self.__map.get_width()):
            for y in range(self.__map.get_height()):
                if self.__map.get(x, y) is not None:
                    global_var.GameMapLayers[defs.LayerEnum.Background].add(self.__map.get(x, y))

        for building in buildings:
            global_var.GameMapLayers[defs.LayerEnum.Building].add(building)

        self.__player = creature.Creature(image=global_var.GameTilesheet.get((0, 0)))
        self.__player.move(player_init_pos.x, player_init_pos.y)
        global_var.GameMapLayers[defs.LayerEnum.Creature].add(self.__player)
        self.__controller = controller.Controller(self.__player)
        self.__camera = camera.Camera()

    def on_update(self, elapse: int):
        self.__controller.update()

        for layer in global_var.GameMapLayers:
            layer.update()

        for c in global_var.GameMapLayers[defs.LayerEnum.Creature]:
            collide.out_of_map_range_fix(self.__map, c)

        self.__camera.stare_at(self.__player.pos.x, self.__player.pos.y,
                               self.__map.get_width(), self.__map.get_height())

    def on_render(self):
        for i in range(len(global_var.GameMapLayers)):
            for sprite in global_var.GameMapLayers[i]:
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
            player_items = self.__player.get_items()
            click_idx = ui.ItemDisplayPanel(self._window,
                                            pygame.Rect(100, 10,
                                                        defs.ScreenWidth - 200, 200),
                                            defs.TileSize,
                                            player_items)
            if click_idx is not None:
                item = player_items[click_idx]
                self.__player.set_hp(self.__player.get_hp() + item.get_hp_recover())
                self.__player.set_nutrition(self.__player.get_nutrition() + item.get_nutrition())
                player_items.pop(click_idx)


if __name__ == '__main__':
    theGame = Game()
    theGame.on_run()
