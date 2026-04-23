#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#define BLOCK_SIZE 30
#define BOARD_W 10
#define BOARD_H 20

extern char board[BOARD_H][BOARD_W];

extern int current_piece, current_rotation, current_x, current_y;
extern int next_piece;

extern int board_start_x, board_start_y;
extern int board_w_pixels, board_h_pixels;
extern int screen_w, screen_h;

void init_board(void);
int check_collision(int kind, int rotation, int px, int py);
void lock_piece(int kind, int rotation, int px, int py);
void clear_lines(void);
void spawn_piece(void);
int try_gravity(void);

#endif
