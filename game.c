#include "board.h"
#include "game.h"
#include <time.h>

#define FPS 60
#define MILLISECONDS 1000
#define FRAMES MILLISECONDS/FPS

Board init_game() {
    Board map;
    make_color_pairs();
    default_board(&map);
    make_board_wins(&map);
    return map;
}

void draw_board(Board* board) {
    box(board->window, 0, 0);
    box(board->status, 0, 0);
    mvwprintw(board->window,0, 0, "frogger");
    mvwprintw(board->status,0, 0, "Status");
    wrefresh(board->window);
    wrefresh(board->status);
}

gameStatus handle_movement(Board* board, char input) {
    switch(input) {
        case 'w':
        case 'k':
            board->frog.posy--;
            if (board->frog.posy <= 0) return VICTORY;  //reached top of the screen - frog has won
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
    return ONGOING;
}

int is_colliding(Board* board) {
    for (int r = 0; r<board->road_amount; r++)  {
        for (int l = 0; l<board->roads[r].lanes; l++) {
            Car current_car = board->roads[r].cars[l];
            if (board->roads[r].posy+l+1 == board->frog.posy) {
                if (board->frog.posx >= current_car.posx &&  board->frog.posx <= current_car.posx+current_car.length) {
                    return DEFEAT;
                }
            }
        }
    }
    return ONGOING;
}

gameStatus main_loop(Board* board) { // will never return ONGOING (because that means the game.. is ongoing...)
    draw_frog(board);
    wtimeout(board->window,(1000/FPS));
    int w;
    int tick_counter = 0;
    clock_t start = clock();
    while (w != KEY_F(1)){
        draw_board(board);
        tick_counter = (tick_counter + 1) % board->tick_speed;
        w = wgetch(board->window);
        clear_frog(board);
        if (handle_movement(board, w) == VICTORY) return VICTORY;
        if (tick_counter == 0) {
            move_cars(board);
        }
        if (is_colliding(board) == DEFEAT) return DEFEAT;
        draw_roads(board);
        draw_cars(board);
        draw_frog(board);
        mvwprintw(board->status, 2, 2, "Time elapsed: %.2lf", (double)(MILLISECONDS*(clock()-start))/((double)FRAMES*CLOCKS_PER_SEC));
        wrefresh(board->window);
        wrefresh(board->status);
    } 
    // user must have pressed f1
    return EXIT; 
}
