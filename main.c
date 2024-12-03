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
    gameStatus result = main_loop(&map);
    print_end_screen(&map, result);
    wrefresh(map.window);
    free_board(&map);
    getchar();
    endwin();
    return 0;
}
