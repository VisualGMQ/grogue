LIB_SRC = $(wildcard lib/*.cpp)
GAME_SRC = $(wildcard game/*.cpp)
SDL_DEP = `sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf
CXX_FLAGS = -std=c++17 -Wextra -Wall

grogue: ${LIB_SRC} ${GAME_SRC}
	$(CXX) $^ -o $@ ${CXX_FLAGS}


.PHONY:clean
clean:
	-rm grogue
