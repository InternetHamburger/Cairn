#include "uci.h"
#include "board.h"
#include "search.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MatchPatterns(char* string, const char* patterns[], int num_patterns, int *patlength) {
    int matched_pattern = -1;
    for (size_t i = 0; i < num_patterns; i++) {
        const size_t patlen = strlen(patterns[i]);
        if (strncmp(string, patterns[i], patlen) == 0) {
            matched_pattern = (int)i;
            *patlength = (int)patlen;
            break;
        }
    }
    return matched_pattern;
}

void SetPosition(char* line, int patlength, int len, Board *board) {
    line += 8 + 1;
    const char* position_types[] = {
        "fen",
        "startpos"
    };
    int num;
    const int pos_type = MatchPatterns(line, position_types, 2, &num);
    if (pos_type == -1) {
        printf("Invalid position type\n");
    }else if (pos_type == 0) {

        // Extract fen
        int empty_encounters = 0;
        int fen_length = 0;
        for (int i = num; i < len; i++){
            if (line[i] == ' ') empty_encounters++;
            if (empty_encounters == 5) {
                fen_length = i + 1 - patlength - num;

                break;
            }
        }
        line += 1 + num;
        printf("%.*s  %d\n", fen_length, line, fen_length);

        *board = BoardConstructor(line, fen_length);
    }else if (pos_type == 1) {
        *board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 55);
    }
}

void GoCommand(char* line, Board *board) {
    const char* go_types[] = {
        "movetime",
        "nodes",
        "depth",
        "wtime" // Assumes btime winc and binc follow
    };
    line += 2 + 1;
    int patlength;
    int go_type = MatchPatterns(line, go_types, 3, &patlength);
    line += 1 + patlength;
    switch (go_type) {
        case -1:
            // Go infinite
            break;
        case 0:
            int time;
            sscanf(line, "%d", &time);
            search(board, -1, -1, time);
            break;
        case 1:
            int nodes;
            sscanf(line, "%d", &nodes);
            search(board, -1, nodes, -1);
            break;
        case 2:
            int depth;
            sscanf(line, "%d", &depth);
            search(board, depth, -1, -1);
            break;
        case 3:
            int white_time;
            int black_time;
            int white_inc;
            int black_inc;
            sscanf(line, "%d", &white_time);
            while (line[0] != ' ') line++;
            line++;
            sscanf(line, "%d", &black_time);
            while (line[0] != ' ') line++;
            line++;
            sscanf(line, "%d", &white_inc);
            while (line[0] != ' ') line++;
            line++;
            sscanf(line, "%d", &black_inc);

            int time_left = board->white_to_move ? white_time : black_time;
            int increment = board->white_to_move ? white_inc : black_inc;

            int time_limit = time_left / 20 + increment / 2;

            search(board, -1, -1, time_limit);
            break;
    }
}

void ReciveCommand(char* line, const int len, Board *board) {
    const char* commands[] = {
        "position",
        "go",
        "isready",
        "quit",
        "uci",
        "ucinewgame"
    };

    int patlength = 0;
    const int matched_pattern = MatchPatterns(line, commands, 3, &patlength);
    printf("%d\n", matched_pattern);
    switch (matched_pattern) {
        case -1:
            printf("Invalid command\n");
            return;
        case 0:
            SetPosition(line, patlength, len, board); // Note! position <fen/startpos> moves not supported
            break;
        case 1:
            GoCommand(line, board);
            break;
        case 2:
            printf("readyok\n");
            return;
        case 3:
            exit(0);
        case 4:
            printf("id name Cairn\nuciok\n");
            break;
        case 5:
            *board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 55);
            break;
    }
}

