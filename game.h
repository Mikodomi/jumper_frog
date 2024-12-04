#ifndef MAIN_GAME_FROGGER_H
#define MAIN_GAME_FROGGER_H
#include "board.h"

typedef enum game_status_enum {
    VICTORY,
    ONGOING,
    DEFEAT,
    EXIT,
} gameStatus;

void draw_board(Board* board);
Board init_game(char* level);
gameStatus main_loop(Board* board);
void print_end_screen(Board* board, gameStatus result);
void game_levels();

#endif
