#include "input.h"
#include "board.h"
#include "pieces.h"
#include "primlib.h"

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
