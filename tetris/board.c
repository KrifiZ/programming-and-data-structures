#include "board.h"
#include "pieces.h"
#include <stdlib.h>

char board[BOARD_H][BOARD_W];

int current_piece, current_rotation, current_x, current_y;
int next_piece;

int block_size;
int board_start_x, board_start_y;
int board_w_pixels, board_h_pixels;
int screen_w, screen_h;

void init_board(void) {
    for (int r = 0; r < BOARD_H; r++)
        for (int c = 0; c < BOARD_W; c++)
            board[r][c] = 0;
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
