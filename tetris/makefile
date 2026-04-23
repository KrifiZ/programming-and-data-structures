CC = gcc
CFLAGS = -g -Wall -pedantic -fsanitize=undefined `sdl2-config --cflags`
LDFLAGS = -g -fsanitize=undefined
LDLIBS = -lSDL2_gfx `sdl2-config --libs` -lm

OBJS = main.o primlib.o pieces.o board.o draw.o input.o

all: main

main: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

primlib.o: primlib.c primlib.h

pieces.o: pieces.c pieces.h

board.o: board.c board.h pieces.h

draw.o: draw.c draw.h board.h pieces.h primlib.h

input.o: input.c input.h board.h pieces.h primlib.h

main.o: main.c primlib.h pieces.h board.h draw.h input.h

clean:
	-rm $(OBJS) main
