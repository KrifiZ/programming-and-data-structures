#include "draw.h"
#include "board.h"
#include "pieces.h"
#include "primlib.h"
#include <string.h>

void draw_block(int x, int y, enum color c) {
    if (x >= 0 && y >= 0) {
        gfx_filledRect(x, y, x + block_size - 1, y + block_size - 1, c);
    }
}

void draw_piece(int kind, int rotation, int px, int py, int ox, int oy) {
    for (int j = 0; j < PIECE_SIZE; j++) {
        for (int i = 0; i < PIECE_SIZE; i++) {
            int block_type = pieces[kind][rotation][j][i];
            if (block_type != 0) {
                enum color c = (block_type == 1) ? GREEN : YELLOW;
                draw_block(ox + (px + i) * block_size,
                           oy + (py + j) * block_size, c);
            }
        }
    }
}

void draw_board(void) {
    for (int row = 0; row < BOARD_H; row++) {
        for (int col = 0; col < BOARD_W; col++) {
            if (board[row][col]) {
                draw_block(board_start_x + col * block_size,
                           board_start_y + row * block_size, RED);
            }
        }
    }
}

void draw_preview(void) {
    int preview_x = board_start_x + board_w_pixels + block_size;
    int preview_y = board_start_y + 10;
    for (int j = 0; j < PIECE_SIZE; j++) {
        for (int i = 0; i < PIECE_SIZE; i++) {
            int block_type = pieces[next_piece][0][j][i];
            if (block_type != 0) {
                enum color c = (block_type == 1) ? GREEN : YELLOW;
                draw_block(preview_x + i * block_size,
                           preview_y + j * block_size, c);
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
