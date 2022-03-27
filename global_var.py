import tilesheet
import world_clock
import pygame


GameTilesheet: tilesheet.Tilesheet = None
GameWorldClock: world_clock.WorldClock = world_clock.WorldClock()
ShowItems: bool = False
Font: pygame.font.Font = None


def toggle_show_items():
    global ShowItems
    ShowItems = not ShowItems
