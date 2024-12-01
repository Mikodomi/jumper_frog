#include "board.h"
#include "game.h"

Board init_game() {
    Board map;
    default_board(&map);
    // parsing config functions will go here
    make_board_win(&map);
    return map;
}

void draw_board(Board* board) {
    box(board->window, 0, 0);
    wprintw(board->window, "frogger");
    wrefresh(board->window);
}

void handle_movement(Board* board, char input) {
    switch(input) {
        case 'w':
        case 'k':
            board->frog.posy--;
            if (board->frog.posy <= 0) board->frog.posy++;
            break;
        case 's':
        case 'j':
            board->frog.posy++;
            if (board->frog.posy >= board->height-1) board->frog.posy--;
            break;
        case 'a':
        case 'h':
            board->frog.posx--;
            if (board->frog.posx <= 0) board->frog.posx++;
            break;
        case 'd':
        case 'l':
            board->frog.posx++;
            if (board->frog.posx >= board->width-1) board->frog.posx--;
            break;
    }
}

void main_loop(Board* board) {
    draw_frog(board);
    wtimeout(board->window,16);
    int w;
    while (w != KEY_F(1)){
        w = wgetch(board->window);
        clear_frog(board);
        handle_movement(board, w);
        draw_roads(board);
        draw_frog(board);
        wrefresh(board->window);
    } 
}
