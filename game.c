#include "board.h"
#include "game.h"

Board init_game() {
    Board map;
    make_color_pairs();
    default_board(&map);
    make_board_win(&map);
    return map;
}

void draw_board(Board* board) {
    box(board->window, 0, 0);
    wprintw(board->window, "frogger");
    wrefresh(board->window);
}

int handle_movement(Board* board, char input) {
    switch(input) {
        case 'w':
        case 'k':
            board->frog.posy--;
            if (board->frog.posy <= 0) return 1;
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
    return 0;
}

int is_colliding(Board* board) {
    for (int r = 0; r<board->road_amount; r++)  {
        for (int l = 0; l<board->roads[r].lanes; l++) {
            Car current_car = board->roads[r].cars[l];
            if (board->roads[r].posy+l+1 == board->frog.posy) {
                if (board->frog.posx >= current_car.posx &&  board->frog.posx <= current_car.posx+current_car.length) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void main_loop(Board* board) {
    draw_frog(board);
    wtimeout(board->window,17);
    int w;
    int tick_counter = 0;
    while (w != KEY_F(1)){
        tick_counter = (tick_counter + 1) % board->tick_speed;
        w = wgetch(board->window);
        clear_frog(board);
        if (handle_movement(board, w)) break;
        if (tick_counter == 0) {
            move_cars(board);
        }
        if (is_colliding(board)) break;
        draw_roads(board);
        draw_cars(board);
        draw_frog(board);
        wrefresh(board->window);
    } 
}
