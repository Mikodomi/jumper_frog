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

typedef struct car_t {
    int posx;
    int color;
    int length;
} Car;

typedef struct road_t {
    int posy;
    int lanes;
    Car* cars;
} Road;

typedef struct board_t {
    WINDOW* window;
    int height;
    int width;
    Frog frog;
    int car_amount;
    int car_length;
    int road_amount;
    Road* roads;
} Board;


void default_board(Board* board);
void make_board_win(Board* board);
void make_color_pairs();

// FROG FUNCTIONS

void clear_frog(Board* board);
void draw_frog(Board* board);

// ROAD FUNCTIONS

void draw_roads(Board* board);

// CAR FUNCTIONS

void generate_cars(Board* board);
void clear_car(Board* board);
void draw_cars(Board* board);

// CLEANING

void free_board(Board* board);


#endif
