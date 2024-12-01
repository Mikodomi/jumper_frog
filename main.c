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
    wattron(map.window, COLOR_PAIR(YELLOW));
    mvwprintw(map.window, map.height/2, (map.width/2)-3, "YOU WIN");
    wrefresh(map.window);
    wattroff(map.window, COLOR_PAIR(YELLOW));
    free_board(&map);
    getchar();
    endwin();
    return 0;
}
