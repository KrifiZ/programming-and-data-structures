#include "primlib.h"
#include "pieces.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BLOCK_SIZE 30
#define BOARD_W 10
#define BOARD_H 20
#define FALL_TICKS 50
#define TICK_DELAY_MS 10

static char board[BOARD_H][BOARD_W];

static int current_piece, current_rotation, current_x, current_y;
static int next_piece;

static int board_start_x, board_start_y;
static int board_w_pixels, board_h_pixels;
static int screen_w, screen_h;

void init_board(void) {
    for (int r = 0; r < BOARD_H; r++)
        for (int c = 0; c < BOARD_W; c++)
            board[r][c] = 0;
}

void draw_block(int x, int y, enum color c) {
    if (x >= 0 && y >= 0) {
        gfx_filledRect(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1, c);
    }
}

void draw_piece(int kind, int rotation, int px, int py, int ox, int oy) {
    for (int j = 0; j < PIECE_SIZE; j++) {
        for (int i = 0; i < PIECE_SIZE; i++) {
            int block_type = pieces[kind][rotation][j][i];
            if (block_type != 0) {
                enum color c = (block_type == 1) ? GREEN : YELLOW;
                draw_block(ox + (px + i) * BLOCK_SIZE,
                           oy + (py + j) * BLOCK_SIZE, c);
            }
        }
    }
}

void draw_board(void) {
    for (int row = 0; row < BOARD_H; row++) {
        for (int col = 0; col < BOARD_W; col++) {
            if (board[row][col]) {
                draw_block(board_start_x + col * BLOCK_SIZE,
                           board_start_y + row * BLOCK_SIZE, RED);
            }
        }
    }
}

void draw_preview(void) {
    int preview_x = board_start_x + board_w_pixels + BLOCK_SIZE;
    int preview_y = board_start_y + 10;
    for (int j = 0; j < PIECE_SIZE; j++) {
        for (int i = 0; i < PIECE_SIZE; i++) {
            int block_type = pieces[next_piece][0][j][i];
            if (block_type != 0) {
                enum color c = (block_type == 1) ? GREEN : YELLOW;
                draw_block(preview_x + i * BLOCK_SIZE,
                           preview_y + j * BLOCK_SIZE, c);
            }
        }
    }
}

void draw_game(void) {
    gfx_filledRect(0, 0, screen_w - 1, screen_h - 1, BLACK);

    gfx_line(board_start_x, board_start_y,
             board_start_x, board_start_y + board_h_pixels, YELLOW);
    gfx_line(board_start_x + board_w_pixels, board_start_y,
             board_start_x + board_w_pixels, board_start_y + board_h_pixels, YELLOW);

    draw_board();
    draw_piece(current_piece, current_rotation, current_x, current_y,
               board_start_x, board_start_y);
    draw_preview();

    gfx_updateScreen();
}

int check_collision(int kind, int rotation, int px, int py) {
    for (int j = 0; j < PIECE_SIZE; j++) {
        for (int i = 0; i < PIECE_SIZE; i++) {
            if (pieces[kind][rotation][j][i] != 0) {
                int bx = px + i;
                int by = py + j;
                if (bx < 0 || bx >= BOARD_W || by >= BOARD_H) {
                    return 1;
                }
                if (by >= 0 && board[by][bx]) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void lock_piece(int kind, int rotation, int px, int py) {
    for (int j = 0; j < PIECE_SIZE; j++) {
        for (int i = 0; i < PIECE_SIZE; i++) {
            if (pieces[kind][rotation][j][i] != 0) {
                int bx = px + i;
                int by = py + j;
                if (bx >= 0 && bx < BOARD_W && by >= 0 && by < BOARD_H) {
                    board[by][bx] = 1;
                }
            }
        }
    }
}

void clear_lines(void) {
    for (int row = BOARD_H - 1; row >= 0; row--) {
        int full = 1;
        for (int col = 0; col < BOARD_W; col++) {
            if (!board[row][col]) {
                full = 0;
                break;
            }
        }
        if (full) {
            for (int r = row; r > 0; r--) {
                for (int c = 0; c < BOARD_W; c++) {
                    board[r][c] = board[r - 1][c];
                }
            }
            for (int c = 0; c < BOARD_W; c++) {
                board[0][c] = 0;
            }
            row++;
        }
    }
}

void spawn_piece(void) {
    current_piece = next_piece;
    current_rotation = 0;
    current_x = BOARD_W / 2 - 2;
    current_y = 0;
    next_piece = rand() % PIECE_COUNT;
}

int handle_input(int key) {
    if (key == SDLK_LEFT) {
        if (!check_collision(current_piece, current_rotation, current_x - 1, current_y)) {
            current_x--;
        }
    }

    if (key == SDLK_RIGHT) {
        if (!check_collision(current_piece, current_rotation, current_x + 1, current_y)) {
            current_x++;
        }
    }

    if (key == SDLK_DOWN) {
        while (!check_collision(current_piece, current_rotation, current_x, current_y + 1)) {
            current_y++;
        }
        lock_piece(current_piece, current_rotation, current_x, current_y);
        clear_lines();
        return 1;
    }

    if (key == SDLK_SPACE) {
        int old_px = 0, old_py = 0;
        for (int j = 0; j < PIECE_SIZE; j++) {
            for (int i = 0; i < PIECE_SIZE; i++) {
                if (pieces[current_piece][current_rotation][j][i] == 2) {
                    old_px = i;
                    old_py = j;
                }
            }
        }

        int next_rot = (current_rotation + 1) % NUM_ROTATIONS;
        int new_px = 0, new_py = 0;
        for (int j = 0; j < PIECE_SIZE; j++) {
            for (int i = 0; i < PIECE_SIZE; i++) {
                if (pieces[current_piece][next_rot][j][i] == 2) {
                    new_px = i;
                    new_py = j;
                }
            }
        }

        int nx = current_x + old_px - new_px;
        int ny = current_y + old_py - new_py;

        if (!check_collision(current_piece, next_rot, nx, ny)) {
            current_rotation = next_rot;
            current_x = nx;
            current_y = ny;
        }
    }

    return 0;
}

int try_gravity(void) {
    if (!check_collision(current_piece, current_rotation, current_x, current_y + 1)) {
        current_y++;
    } else {
        lock_piece(current_piece, current_rotation, current_x, current_y);
        clear_lines();
        spawn_piece();
        if (check_collision(current_piece, current_rotation, current_x, current_y)) {
            return 1;
        }
    }
    return 0;
}

void show_game_over(void) {
    gfx_filledRect(0, 0, screen_w - 1, screen_h - 1, BLACK);

    gfx_line(board_start_x, board_start_y,
             board_start_x, board_start_y + board_h_pixels, YELLOW);
    gfx_line(board_start_x + board_w_pixels, board_start_y,
             board_start_x + board_w_pixels, board_start_y + board_h_pixels, YELLOW);

    draw_board();

    const char *msg = "GAME OVER";
    int text_x = board_start_x + (board_w_pixels - 8 * (int)strlen(msg)) / 2;
    int text_y = board_start_y + board_h_pixels / 2 - 4;
    gfx_textout(text_x, text_y, msg, YELLOW);

    gfx_updateScreen();

    while (1) {
        int k = gfx_getkey();
        if (k == SDLK_RETURN || k == SDLK_ESCAPE) {
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    if (gfx_init()) {
        exit(3);
    }

    srand(time(NULL));
    init_board();

    screen_w = gfx_screenWidth();
    screen_h = gfx_screenHeight();

    board_w_pixels = BOARD_W * BLOCK_SIZE;
    board_h_pixels = BOARD_H * BLOCK_SIZE;

    board_start_x = (screen_w - board_w_pixels) / 2;
    board_start_y = (screen_h - board_h_pixels) / 2;
    if (board_start_y < 0) board_start_y = 0;

    next_piece = rand() % PIECE_COUNT;
    spawn_piece();

    int tick = 0;
    int game_over = 0;

    while (1) {
        int key = gfx_pollkey();

        if (key == SDLK_ESCAPE) {
            break;
        }

        if (key != -1) {
            int dropped = handle_input(key);
            if (dropped) {
                spawn_piece();
                if (check_collision(current_piece, current_rotation, current_x, current_y)) {
                    game_over = 1;
                    break;
                }
                tick = 0;
            }
        }

        draw_game();

        tick++;
        if (tick >= FALL_TICKS) {
            tick = 0;
            if (try_gravity()) {
                game_over = 1;
                break;
            }
        }

        SDL_Delay(TICK_DELAY_MS);
    }

    if (game_over) {
        show_game_over();
    }

    return 0;
}
