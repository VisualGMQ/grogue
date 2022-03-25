import pygame
import tile
import food
import defs


FoodInfoTable = {
    'berry': ((2, 2), food.FoodConstitution(10, 0, 90))
}


class Plant(tile.Tile):
    def __init__(self, grown_threshold: int,
                 groth_tile: pygame.Surface.subsurface,
                 grown_tile: pygame.Surface.subsurface):
        tile.Tile.__init__(self, groth_tile)

        self.growth_tile = groth_tile
        self.grown_tile = pygame.transform.scale(grown_tile,
                                                 (defs.TileSize,
                                                  defs.TileSize))
        self.growth = 0
        self.grown_threshold = grown_threshold

    def grow(self, growth: int):
        self.growth = min(self.grown_threshold, self.growth + growth)
        if self.growth >= self.grown_threshold:
            self.image = self.grown_tile

    def pick(self) -> food.Food:
        self.growth = 0
        self.image = self.growth_tile

        berry = FoodInfoTable['berry']
        return food.Food(berry[0], berry[1])


PlantInfoTable = {
    'berry cluster': {
        'threshold': 100,
        'tile_pos': (0, 2),
        'growth_tile_pos': (1, 2),
    }
}
