#include "board.h"
#include "parsing.h"
#include <ncurses.h>
#include <stdlib.h>


void default_board(Board* board) {
    board->height = 30;
    board->width = 30;
    board->frog.size = 1;
    board->frog.posx = board->width/2;
    board->frog.posy = board->height-2;
    board->car_amount = 5;
    board->car_length = 3;
    board->road_amount = 0;
}

//config functions here (none for now)
//...
// ----- 



void make_board_win(Board* board) {
    parse_config(board);
    board->window = newwin(board->height, board->width, 0, 0);
    generate_cars(board);
}

void make_color_pairs() {
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
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
    printw("segfault");
    for (int r = 0; r<board->road_amount; r++) { // r for road    
        board->roads[r].cars = (Car*)malloc(board->roads[r].lanes*sizeof(Car));
            if (board->roads[r].cars == NULL) {
                break;
            }
        for (int l = 0; l<board->roads[r].lanes; l++) { // l for lanes
            Car* current_car = &(board->roads[r].cars[l]);
            current_car->posx = rand() % board->width-1;
            current_car->color = (rand() % 4) + 2;
            current_car->length = 3;
        }
    }
}

void clear_car(Board* board) {

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

void free_board(Board* board) {
    free(board->roads);
}
