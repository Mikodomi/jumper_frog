#ifndef FROGGER_BOARD_H
#define FROGGER_BOARD_H
#include <ncurses.h>

typedef struct frog_t {
    int posx;
    int posy; // position of the center
    int size; // a radius
              // e.g. size = 1 is 1x1
              // but size = 2 is 3x3, because the "radius" from the center is 2
    int topy;
} Frog;

typedef enum color_en {
    BLACK,
    GREEN,
    RED,
    BLUE,
    YELLOW,
    MAGENTA,
} Color;

typedef struct car_t {
    int posx;
    Color color;
    int length;
    int direction;
} Car;

typedef struct road_t {
    int posy;
    int lanes;
    Car* cars;
} Road;

typedef struct stork_t {
    int posx;
    int posy;
    int speed; // speed = n --> stork moves every n car ticks
} Stork;

typedef struct board_t {
    WINDOW* window;
    WINDOW* status;
    int height;
    int width;
    Frog frog;
    Stork stork;
    int car_amount;
    int car_length;
    int road_amount;
    Road* roads;
    int tick_speed;
    int disappear_chance; // into the border
    int switch_chance; // switch to be a different color and go back instead of wrap
                       // by extension, the chance to wrap around the map is 100% - switch_chance%
                       // warning: this probability stacks with the disappear_chance
                       // so overall probability for a car to do this is (disappear_chance*switch_chance)/100^2 (small)
                       // make this pretty big
    int can_boost;
    int score;
    double time_score;
    int move_score;
    int* obstacles;
} Board;


void default_board(Board* board);
void make_board_wins(Board* board, char* level);
void make_color_pairs();

// FROG FUNCTIONS

void clear_frog(Board* board);
void draw_frog(Board* board);

// ROAD FUNCTIONS

void draw_roads(Board* board);

// CAR FUNCTIONS

int car_obstacle_collision(Board* board, int posy, int l, Car* current_car);
void generate_cars(Board* board);
void move_cars(Board* board, int tick_count);
void draw_cars(Board* board);
int frog_proximity(Board* board, int posy, int l, Car* current_car);

// STORK FUNCTIONS

void move_stork(Board* board);
void draw_stork(Board* board);

// CLEANING

void free_board(Board* board);


#endif
