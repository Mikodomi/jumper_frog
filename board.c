#include "board.h"
#include <ncurses.h>
#include <stdlib.h>

void make_roads(Board* board) {
    
}

void default_board(Board* board) {
    board->height = 30;
    board->width = 30;
    board->frog.size = 1;
    board->frog.posx = board->width/2;
    board->frog.posy = board->height-2;
    board->car_amount = 5;
    board->car_length = 3;
    board->road_amount = 1;
    board->roads[0].lanes = 3;
    board->roads[0].posy = 15;
}

//config functions here (none for now)
//...
// ----- 



void make_board_win(Board* board) {
    board->window = newwin(board->height, board->width, 0, 0);
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

void clear_car(Board* board) {

}

void draw_car(Board* board) {
    
}

