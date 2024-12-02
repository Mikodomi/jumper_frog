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
    switch (result) {
        case VICTORY:
            wattron(map.window, COLOR_PAIR(YELLOW));
            mvwprintw(map.window, map.height/2, (map.width/2)-3, "YOU WIN");
            wattroff(map.window, COLOR_PAIR(YELLOW));
            break;
        case DEFEAT:
            wattron(map.window, COLOR_PAIR(RED));
            mvwprintw(map.window, map.height/2, (map.width/2)-4, "YOU LOSE");
            wattroff(map.window, COLOR_PAIR(RED));
            break;
        case EXIT:
            wattron(map.window, COLOR_PAIR(GREEN));
            mvwprintw(map.window, map.height/2, (map.width/2)-4, "YOU EXIT");
            wattroff(map.window, COLOR_PAIR(GREEN));
            break;
        case ONGOING:
            //should never happen
            mvwprintw(map.window, 0, 0, "how did we get here?");
            break;
        }
    wrefresh(map.window);
    free_board(&map);
    getchar();
    endwin();
    return 0;
}
