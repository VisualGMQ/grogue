class WorldClock:
    def __init__(self):
        self.curTime = 0
        self.__elapse = 0

    def elapse(self, elapse: int):
        self.curTime += elapse
        self.__elapse = elapse

    def get_elapse(self):
        return self.__elapse

    def reset(self):
        self.curTime = 0
