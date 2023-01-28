.PHONY: all launch clean

all: game

clean:
	@rm -f gamebin *.o
	@rm -rf gamebin.dSYM

game: ./gamebin

launch: game
	./gamebin

./gamebin: main.cpp
	@clang++ -I./ window.cpp entity.cpp main.cpp -o gamebin $(shell pkg-config sdl2 sdl2_image sdl2_mixer sdl2_ttf --cflags --libs) -g -Wall -std=c++17
