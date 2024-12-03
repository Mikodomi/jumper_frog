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

void print_obstacles(Board* board) {
    for (int h = 0; h<board->height; h++) {
        for (int w = 0; w<board->width; w++) {
            if (board->obstacles[(h*board->width)+w]) {
                mvwprintw(board->window, h, w, "&");
            }
        }
    }
}

void draw_board(Board* board) {
    box(board->window, 0, 0);
    box(board->status, 0, 0);
    mvwprintw(board->window,0, 0, "frogger");
    mvwprintw(board->status,0, 0, "Status");
    wrefresh(board->window);
    wrefresh(board->status);
}

int frog_obstacle_collision(Board* board) {
    return board->obstacles[(board->frog.posy*board->width)+board->frog.posx];
}

gameStatus handle_movement(Board* board, char input) {
    int old_posy = board->frog.posy;
    int old_posx = board->frog.posx;
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
        case 'b':
            if (board->can_boost) {
                board->frog.posy = board->can_boost;
                board->can_boost = 0;
            }
            break;
    }
    if (frog_obstacle_collision(board)) {
        board->frog.posx = old_posx;
        board->frog.posy = old_posy;
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
    double time_elapsed;
    int speed_change = 1;
    while (w != KEY_F(1)){
        draw_board(board);
        print_obstacles(board);
        tick_counter = (tick_counter + 1) % board->tick_speed;
        w = wgetch(board->window);
        clear_frog(board);
        if (handle_movement(board, w) == VICTORY) return VICTORY;
        if (tick_counter == 0 || tick_counter == (board->tick_speed)/2) {
            move_cars(board, tick_counter);
        }
        if (is_colliding(board) == DEFEAT) return DEFEAT;
        draw_roads(board);
        draw_cars(board);
        draw_frog(board);
        time_elapsed = (double)(MILLISECONDS*(clock()-start))/((double)FRAMES*CLOCKS_PER_SEC);
        if (time_elapsed > 3 && speed_change) {
            board->tick_speed--;
            speed_change = 0;
        }
        mvwprintw(board->status, 2, 2, "Time elapsed: %.2lf", time_elapsed);
        wrefresh(board->window);
        wrefresh(board->status);
    } 
    // user must have pressed f1
    return EXIT; 
}
