#ifndef DRAW_H
#define DRAW_H

#include "primlib.h"

void draw_block(int x, int y, enum color c);
void draw_piece(int kind, int rotation, int px, int py, int ox, int oy);
void draw_board(void);
void draw_preview(void);
void draw_game(void);
void show_game_over(void);

#endif
