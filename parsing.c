#include <string.h>
#include <stdlib.h>
#include "parsing.h"
#include "board.h"

typedef enum parse_error_enum {
    PARSE_EOF,
    PARSE_ERROR,
    PARSE_SUCCESS,
    PARSE_BAD_MALLOC,
} parseError;

parseError frog_parse(Board* board, FILE* file) {
    char buffer[32];
    while (fgets(buffer, 32, file)) {
        char command[32];
        int value;
        sscanf(buffer,"%s %d", command, &value);
        if (strcmp(command, "size") == 0) {
            board->frog.size = value;
        }
        if (strcmp(command, "color") == 0) {
            // todo: color table
        }
    }
    if (!fgets(buffer, 32, file)) return PARSE_EOF;
    return PARSE_SUCCESS;
}

parseError roads_parse(Board* board, FILE* file) {
    int amount;
    fscanf(file, "%d ", &amount);
    board->road_amount = amount;
    wrefresh(board->window);
    board->roads = (Road*)malloc(amount*sizeof(Road));
    if (board->roads == NULL) return PARSE_BAD_MALLOC;
    for (int i = 0; i<amount; i++) {
        int status = fscanf(file, "%d %d ", &(board->roads[i].posy), (&board->roads[i].lanes));  
        switch(status) {
            case EOF:
                board-> road_amount = 0;
                free(board->roads);
                return PARSE_EOF;
            case 0:
                board->road_amount = 0;
                free(board->roads);
                return PARSE_ERROR;
            }
    }
    return PARSE_SUCCESS;
}

void parse_config(Board* board) {
    FILE* file = fopen("config.txt", "r");
    if (file == NULL) return; // default board is given for this case
    char buffer[32];
    while(fgets(buffer, 32, file) != NULL) { // expects newline character after the main command (e.g. "FROG\nsize"...)
        char command[32];
        sscanf(buffer, "%s", command);
        if (strcmp(command, "FROG") == 0) {
            frog_parse(board, file);
        }
        if (strcmp(command, "ROADS") == 0) {
            roads_parse(board, file);
        }
    }
    wprintw(board->window, "file parsing completed");
    fclose(file);
}
