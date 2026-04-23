#include "primlib.h"
#include "pieces.h"
#include "board.h"
#include "draw.h"
#include "input.h"
#include <stdlib.h>
#include <time.h>

#define FALL_TICKS 50
#define TICK_DELAY_MS 10

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
