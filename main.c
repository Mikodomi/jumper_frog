#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
    srand(time(NULL));
    initscr();
    noecho();
    curs_set(0);
    start_color();

    refresh();
    Board map = init_game();
    keypad(map.window, TRUE);
    draw_board(&map);
    main_loop(&map);
    free_board(&map);
    endwin();
    return 0;
}
