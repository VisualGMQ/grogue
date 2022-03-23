import sys
import pygame
from pygame.locals import *
import key


class GameTemplate:
    def on_init(self):
        pygame.init()

        self._running = True
        self._fps_ticker = pygame.time.Clock()
        self._window = pygame.display.set_mode((1024, 720))
        pygame.display.set_caption('Grogue prototype')

    def on_quit(self):
        pygame.quit()

    def on_event(self, event: pygame.event.Event):
        if event.type == pygame.QUIT:
            self._running = False
            sys.exit()
        if event.type == pygame.KEYDOWN:
            key.Key.update(True, event.__dict__['key'])
        if event.type == pygame.KEYUP:
            key.Key.update(False, event.__dict__['key'])

    def on_update(self, elapse: int):
        pass

    def on_render(self):
        pass

    def on_run(self):
        self.on_init()
        t = 0
        while self._running:
            self._window.fill((0, 0, 0, 255))
            key.Key.update_old_keys()
            for event in pygame.event.get():
                self.on_event(event)
            elapse = pygame.time.get_ticks() - t
            elapse = min(elapse, 30)
            t = pygame.time.get_ticks()
            self.on_update(elapse)
            self.on_render()
            pygame.display.flip()
            self._fps_ticker.tick(60)
        self.on_quit()
