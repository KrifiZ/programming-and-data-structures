CC = gcc
CFLAGS = -g -Wall -pedantic -fsanitize=undefined $(shell pkg-config --cflags sdl2 SDL2_gfx)
LDLIBS = -fsanitize=undefined $(shell pkg-config --libs sdl2 SDL2_gfx) -lm

all: hanoi

hanoi: hanoi.o primlib.o
	$(CC) $^ -o $@ $(LDLIBS)

hanoi.o: hanoi.c hanoi.h primlib.h
	$(CC) $(CFLAGS) -c hanoi.c

primlib.o: primlib.c primlib.h
	$(CC) $(CFLAGS) -c primlib.c

clean:
	rm -f hanoi *.o