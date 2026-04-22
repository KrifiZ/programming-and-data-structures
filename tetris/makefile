all: main

main: main.o primlib.o pieces.o
	gcc -g -fsanitize=undefined $^ -o $@  -lSDL2_gfx `sdl2-config --libs` -lm

.c.o: 
	gcc -g -Wall -pedantic -fsanitize=undefined `sdl2-config --cflags` -c  $<

primlib.o: primlib.c primlib.h

main.o: main.c primlib.h pieces.h

pieces.o: pieces.c pieces.h

clean:
	-rm primlib.o main.o pieces.o main