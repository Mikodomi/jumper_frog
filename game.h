#ifndef MAIN_GAME_FROGGER_H
#define MAIN_GAME_FROGGER_H
#include "board.h"

void draw_board(Board* board);
Board init_game();
void main_loop(Board* board);

#endif
