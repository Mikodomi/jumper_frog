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
        sscanf(buffer,"%s %d ", command, &value);
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

parseError cars_parse(Board* board, FILE* file) {
    int amount; // amont of commands
    fscanf(file, "%d ", &amount);
    char buffer[32];
    int value;
    for (int i = 0; i<amount; i++) {
        fscanf(file, "%s %d ", buffer, &value);
        if (strcmp(buffer, "length") == 0) {
            board->car_length = value;
        }
        if (strcmp(buffer, "tick_speed") == 0) {
            board->tick_speed = value;
        }
        if (strcmp(buffer, "disappear_chance") == 0) {
            board->disappear_chance = value;
        }
        if (strcmp(buffer, "switch_chance") == 0) {
            board->switch_chance = value;
        }
    }
    return PARSE_SUCCESS;
}

parseError obstacles_parse(Board* board, FILE* file) {
    int amount; // amount of obstacles
    fscanf(file, "%d ", &amount);
    for (int i = 0; i<amount; i++) {
        int x, y;
        int status = fscanf(file, "%d %d ", &x, &y);
        switch (status) {
            case EOF:
                return PARSE_EOF;
            case 0:
                return PARSE_ERROR;
        }
        board->obstacles[(y+1)*board->width+x] = 1;
    }
    return PARSE_SUCCESS;
}

parseError stork_parse(Board* board, FILE* file) {
    char buffer[32];
    int speed;
    int status = fscanf(file, "%s %d ", buffer, &speed);
    switch (status) {
        case EOF:
            return PARSE_EOF;
        case 0:
            return PARSE_ERROR;
        }
    board->stork.speed = speed;
    return PARSE_SUCCESS;
}

void parse_config(Board* board, char* level) {
    FILE* file = fopen(level, "r");
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
        if (strcmp(command, "CARS") == 0) {
            cars_parse(board, file);
        }
        if (strcmp(command, "OBSTACLES") == 0) {
            obstacles_parse(board, file);
        }
        if (strcmp(command, "STORK") == 0) {
            stork_parse(board, file);
        }
    }
    fclose(file);
}
