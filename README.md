# Programming and Data Structures - Lab Projects

A collection of graphical programs written in C using the SGL (Simple Graphics Library) built on SDL2.

## Projects

### Rotating Polygon (`polygon/`)

A program that displays a smoothly rotating polygon that grows and shrinks each frame. The rotation angle is kept in the range [0, 2pi) to prevent overflow.

**Configuration** (compile-time constants in `main.c`):

| Constant        | Default | Description                         |
| --------------- | ------- | ----------------------------------- |
| `NUM_VERTICES`  | 5       | Number of polygon vertices          |
| `ROTATION_STEP` | 0.03    | Angle increment per frame (radians) |

**Controls:** Press any key to exit.

### Tower of Hanoi (`hanoi/`)

A graphical Tower of Hanoi game with animated disc movement. The player moves discs between pegs following the classic rules -- only one disc at a time, and a larger disc cannot be placed on a smaller one. The goal is to move all discs from the first peg to any other peg.

All dimensions are fully parametrized -- changing the number of discs or pegs automatically recalculates the layout. Disc movements use a smooth three-phase animation (up, across, down).

**Configuration** (compile-time constants in `hanoi.h`):

| Constant         | Default | Description                         |
| ---------------- | ------- | ----------------------------------- |
| `NUM_PEGS`       | 5       | Number of pegs (supports up to 10)  |
| `NUM_DISCS`      | 8       | Number of discs                     |
| `ANIMATION_STEP` | 10      | Pixel step per frame for animations |

**Controls:**

| Key          | Action                                                   |
| ------------ | -------------------------------------------------------- |
| `1`-`9`, `0` | Select peg (1st press = source, 2nd press = destination) |
| `Escape`     | Quit the game                                            |
| `Enter`      | Quit the game                                            |

A "Congratulation" message is displayed when the puzzle is solved.

### Tetris (`tetris/`)

A graphical implementation of the classic Tetris game. Pieces fall from the top of the screen, and the player must rotate and move them to form complete rows. Completed rows are cleared, and the game speeds up as the player progresses.

**Configuration** (compile-time constants in `pieces.h`):

| Constant       | Default | Description                      |
| -------------- | ------- | -------------------------------- |
| `BOARD_WIDTH`  | 10      | Number of columns on the board   |
| `BOARD_HEIGHT` | 20      | Number of rows on the board      |
| `DROP_SPEED`   | 500     | Initial drop speed (ms per step) |

**Controls:**

| Key           | Action            |
| ------------- | ----------------- |
| `Arrow Left`  | Move piece left   |
| `Arrow Right` | Move piece right  |
| `Arrow Up`    | Rotate piece      |
| `Arrow Down`  | Drop piece faster |
| `Space`       | Hard drop         |
| `Escape`      | Quit the game     |

## Building

Requires GCC, SDL2, and SDL2_gfx.

```sh
cd polygon && make
cd hanoi && make
cd tetris && make
```

Compiler flags: `-g -Wall -pedantic`. The Hanoi project additionally enables `-fsanitize=undefined` for boundary error checking.

## Running

```sh
./polygon/polygon
./hanoi/hanoi
./tetris/tetris
```
