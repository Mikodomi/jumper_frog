#include "board.h"
#include "parsing.h"
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926

void fill_obstacles_matrix(Board* board);

void default_board(Board* board) {
    board->height = 30;
    board->width = 50;
    board->frog.size = 1;
    board->frog.posx = board->width/2;
    board->frog.posy = board->height-2;
    board->frog.topy = board->height-2;
    board->stork.posx = 1;
    board->stork.posy = 1;
    board->stork.speed = 20;
    board->car_amount = 5;
    board->car_length = 3;
    board->road_amount = 0;
    board->tick_speed = 3;
    board->disappear_chance = 90;
    board->switch_chance = 50;
    board->obstacles = (int*)malloc(sizeof(int)*board->height*board->width);
    board->can_boost = 0;
    board->score = 0;
    board->move_score = 0;
    board->time_score = 100;
    fill_obstacles_matrix(board);
}

void fill_obstacles_matrix(Board* board) {
    for (int h = 0; h<board->height; h++) {
        for (int w = 0; w<board->width; w++) {
            board->obstacles[(h*board->width)+w] = 0;
        }
    }
}


void make_board_wins(Board* board) {
    parse_config(board);
    board->window = newwin(board->height, board->width, 0, 0);
    board->status = newwin(5, 25, 0, board->width+1);
    generate_cars(board);
}

void make_color_pairs() {
    init_pair(0, COLOR_BLACK, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    //init_pair(2, COLOR_BLACK, COLOR_RED);
    //init_pair(2, COLOR_BLACK, COLOR_RED);
}

void clear_frog(Board* board) {
    mvwprintw(board->window, board->frog.posy, board->frog.posx, " ");
}


void draw_frog(Board* board) {
    clear_frog(board);
    wattron(board->window, COLOR_PAIR(1));
    mvwprintw(board->window, board->frog.posy, board->frog.posx, "F");
    wattroff(board->window,COLOR_PAIR(1));
    wrefresh(board->window);
}

void draw_road(Board* board, Road road) {
    for (int i = 1; i<board->width-1; i++) {
        mvwprintw(board->window, road.posy, i, "_");
        mvwprintw(board->window, road.posy+road.lanes, i, "_");
        if (i % 2 == 0) {
            for (int l = 0; l<road.lanes; l++) {
                mvwprintw(board->window, road.posy+l, i, "_");
            }
        }
    }
}

void draw_roads(Board* board) {
    for (int i = 0; i<board->road_amount; i++) {
        draw_road(board, board->roads[i]);
    }
}

// cars


void generate_cars(Board* board) {
    for (int r = 0; r<board->road_amount; r++) { // r for road    
        board->roads[r].cars = (Car*)malloc(board->roads[r].lanes*sizeof(Car));
            if (board->roads[r].cars == NULL) {
                break;
            }
        for (int l = 0; l<board->roads[r].lanes; l++) { // l for lanes
            Car* current_car = &(board->roads[r].cars[l]);
            current_car->length = board->car_length;
            do {
            current_car->posx = (rand() % (board->width-current_car->length-2))+1;
            } while (car_obstacle_collision(board, board->roads[r].posy+1,l,current_car));
            current_car->color = (Color)(rand() % 4) + 1;
            current_car->direction = ((rand() % 2) == 0 ? 1 : -1);
        }
    }
}

void clear_car(Board* board,int road, int lane,  Car* current_car) {  
            wattron(board->window, COLOR_PAIR(0));
            for (int len = 0; len<current_car->length; len++) {
               mvwprintw(board->window, board->roads[road].posy+lane+1, current_car->posx+len, " "); 
            }
            wattroff(board->window, COLOR_PAIR(0));
}

void wrap_or_switch(Board* board, Car* current_car, int roll) {
            if (current_car->posx < -1) {
                if (roll <= 100-board->switch_chance) {
                    current_car->posx = board->width+1; 
                } else {
                    current_car->color = (Color)(rand() % 4) + 1;
                    current_car->direction = 1;
                }
            } else if (current_car->posx > board->width+1) {
                if (roll <= 100-board->switch_chance) {
                    current_car->posx = -1; 
                } else {
                    current_car->color = (Color)(rand() % 4) + 1;
                    current_car->direction = -1;
                }
            }
}

int car_obstacle_collision(Board* board, int posy, int lane, Car* current_car) { 
    return board->obstacles[((posy+lane)*board->width)+current_car->posx] || board->obstacles[((posy+lane)*board->width)+current_car->posx+current_car->length]; // BEAST but saves space overall
}

int frog_proximity(Board* board, int posy, int lane, Car* current_car) { // used for 2 car types
    return (board->frog.posx >= current_car->posx) &&
           (board->frog.posx <= current_car->posx + current_car->length) &&
           ((board->frog.posy-2 == posy+lane) || board->frog.posy == posy+lane);
}

void move_cars(Board* board, int tick_count) {
    board->can_boost = 0;
    for (int r = 0; r<board->road_amount; r++) {
        for (int l = 0; l<board->roads[r].lanes; l++) {
            Car* current_car = &(board->roads[r].cars[l]);
            if (tick_count == 0 && current_car->color != RED) {
                continue;
            }
            if (frog_proximity(board, board->roads[r].posy, l, current_car) && current_car->color == BLUE) {
                continue;
            }
            
            if (frog_proximity(board, board->roads[r].posy, l, current_car) && current_car->color == GREEN) {
                board->can_boost = board->roads[r].posy;
            }
            int oldx = current_car->posx;
            clear_car(board, r, l, current_car);
            current_car->posx += current_car->direction;
            int roll = rand() % 100; // for determining if the car bounces off the wall
                                     // or (eventually) wraps around the other side
            if (current_car->posx == 1) {
                if (roll < 100-board->disappear_chance) {
                    current_car->direction = 1;
                }
            } else if (current_car->posx == (board->width-current_car->length)-1){
                if (roll < 100-board->disappear_chance) {
                    current_car->direction = -1;
                }
            }
            if (car_obstacle_collision(board, board->roads[r].posy+1, l, current_car)) {
                current_car->posx = oldx;
                current_car->direction *= -1;
            }
            // wrapping or switching
            wrap_or_switch(board, current_car, roll);
        }
    }
}

void draw_cars(Board* board) {
    for (int r = 0; r<board->road_amount; r++) { // r for road    
        for (int l = 0; l<board->roads[r].lanes; l++) { // l for lanes
            Car current_car = board->roads[r].cars[l];
            wattron(board->window, COLOR_PAIR(current_car.color));
            for (int len = 0; len<current_car.length; len++) {
               mvwprintw(board->window, board->roads[r].posy+l+1, current_car.posx+len, " "); 
            }
            wattroff(board->window, COLOR_PAIR(current_car.color));
        }
    }
}

void move_stork(Board* board) {
    mvwprintw(board->window, board->stork.posy, board->stork.posx, " ");
    if (board->stork.posx != board->frog.posx) {
            board->stork.posx += (board->frog.posx > board->stork.posx) ? 1 : -1;    
    }
    if (board->stork.posy != board->frog.posy) {
        board->stork.posy += (board->frog.posy > board->stork.posy) ? 1 : -1;
    }
}

void draw_stork(Board* board) {
    wattron(board->window, COLOR_PAIR(MAGENTA));
    mvwprintw(board->window, board->stork.posy, board->stork.posx, "S");
    wattroff(board->window, COLOR_PAIR(MAGENTA));
}

void free_board(Board* board) {
    for (int r = 0; r<board->road_amount; r++) {
        free(board->roads[r].cars);
    }
    free(board->roads);
    free(board->obstacles);
    delwin(board->window);
    delwin(board->status);
}
