#ifndef FROGGER_BOARD_H
#define FROGGER_BOARD_H
#include <ncurses.h>

typedef struct frog_t {
    int posx;
    int posy;
    int size; // a radius
              // e.g. size = 1 is 1x1
              // but size = 2 is 3x3, because the "radius" from the center is 2
} Frog;

typedef struct board_t {
    WINDOW* window;
    int height;
    int width;
    Frog frog;
    int car_amount;
    int car_length;
} Board;

void default_board(Board* board);
void make_board_win(Board* board);

// FROG FUNCTIONS

void clear_frog(Board* board);
void draw_frog(Board* board);

#endif
