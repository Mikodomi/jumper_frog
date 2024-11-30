#ifndef FROGGER_BOARD_H
#define FROGGER_BOARD_H
#include <ncurses.h>

typedef struct frog_t {
    int posx;
    int posy; // position of the center
    int size; // a radius
              // e.g. size = 1 is 1x1
              // but size = 2 is 3x3, because the "radius" from the center is 2
} Frog;

typedef struct road_t {
    int posy;
    int lanes;
} Road;

typedef struct board_t {
    WINDOW* window;
    int height;
    int width;
    Frog frog;
    int car_amount;
    int car_length;
    int road_amount;
    Road roads[1];
} Board;


void default_board(Board* board);
void make_board_win(Board* board);

// FROG FUNCTIONS

void clear_frog(Board* board);
void draw_frog(Board* board);

// ROAD FUNCTIONS

void draw_roads(Board* board);

// CAR FUNCTIONS

void clear_car(Board* board);
void draw_car(Board* board);



#endif
