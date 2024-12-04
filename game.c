#include "board.h"
#include "game.h"
#include <time.h>
#include <math.h>

#define FPS 60
#define MILLISECONDS 1000
#define FRAMES MILLISECONDS/FPS
#define PI 3.14159265

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
            if (board->frog.posy <= 0) {
                board->score += 10;
                return VICTORY;  //reached top of the screen - frog has won
            }
            if (board->frog.posy < board->frog.topy) {
                board->move_score++;
                board->frog.topy--;
            }
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
    if (board->stork.posx == board->frog.posx && board->stork.posy == board->frog.posy) {
        return DEFEAT;
    }
    return ONGOING;
}

void calculate_score(Board* board, double time) {
    board->time_score = ((PI/2)-atan((0.3*time)-2))*34 + 10; // converting to arcctg (polish verion)
    board->score = (int)(board->time_score) + board->move_score;
}

void clear_status(Board* board) {
    for (int i = 2; i<4; i++) {
        for (int j = 2; j<15; j++) {
            mvwprintw(board->status, i, j, " ");
        }
    }
}

gameStatus main_loop(Board* board) { // will never return ONGOING (because that means the game.. is ongoing...)
    draw_frog(board);
    wtimeout(board->window,(1000/FPS));
    int w;
    int tick_counter = 0;
    clock_t start = clock();
    double time_elapsed;
    int speed_change = 1;
    int stork_count = 0;
    while (w != KEY_F(1)){
        draw_board(board);
        print_obstacles(board);
        tick_counter = (tick_counter + 1) % board->tick_speed;
        w = wgetch(board->window);
        clear_frog(board);
        if (handle_movement(board, w) == VICTORY) return VICTORY;
        if (tick_counter == 0 || tick_counter == (board->tick_speed)/2) {
            move_cars(board, tick_counter);
            stork_count = (stork_count + 1) % board->stork.speed;
        }
        if (stork_count == 0) {
            move_stork(board);
        }
        if (is_colliding(board) == DEFEAT) return DEFEAT;
        draw_roads(board);
        draw_cars(board);
        draw_frog(board);
        draw_stork(board);
        time_elapsed = (double)(MILLISECONDS*(clock()-start))/((double)FRAMES*CLOCKS_PER_SEC);
        if (time_elapsed > 30 && speed_change) {
            board->tick_speed--;
            speed_change = 0;
        }
        calculate_score(board, time_elapsed);
        clear_status(board);
        mvwprintw(board->status, 2, 2, "Time elapsed: %.2lf", time_elapsed);
        mvwprintw(board->status, 3, 2, "Score: %d", board->score);
        wrefresh(board->window);
        wrefresh(board->status);


    } 
    // user must have pressed f1
    return EXIT; 
}

void print_end_screen(Board* board, gameStatus result) {
    switch (result) {
        case VICTORY:
            wattron(board->window, COLOR_PAIR(YELLOW));
            mvwprintw(board->window, board->height/2, (board->width/2)-3, "  YOU WIN  ");;
            mvwprintw(board->window, (board->height/2)-1, (board->width/2)-3, "  Score:%d  ", board->score);
            wattroff(board->window, COLOR_PAIR(YELLOW));
            break;
        case DEFEAT:
            wattron(board->window, COLOR_PAIR(RED));
            mvwprintw(board->window, board->height/2, (board->width/2)-4, "YOU LOSE");
            wattroff(board->window, COLOR_PAIR(RED));
            break;
        case EXIT:
            wattron(board->window, COLOR_PAIR(GREEN));
            mvwprintw(board->window, board->height/2, (board->width/2)-4, "YOU EXIT");
            wattroff(board->window, COLOR_PAIR(GREEN));
            break;
        case ONGOING:
            //should never happen
            mvwprintw(board->window, 0, 0, "how did we get here?");
            break;
        }
}
