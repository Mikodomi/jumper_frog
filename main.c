#include <ncurses.h>
#include "game.h"

int main() {
    initscr();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);

    refresh();
    Board map = init_game();
    keypad(map.window, TRUE);
    draw_board(&map);
    main_loop(&map);
    wrefresh(map.window);
    endwin();
    return 0;
}
