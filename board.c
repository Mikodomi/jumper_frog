#include "board.h"
#include <ncurses.h>

void default_board(Board* board) {
    board->height = 30;
    board->width = 30;
    board->frog.size = 1;
    board->frog.posx = board->width/2;
    board->frog.posy = 0;
    board->car_amount = 5;
    board->car_length = 3;
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


