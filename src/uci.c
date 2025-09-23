#include "uci.h"
#include "board.h"
#include "search.h"
#include "utility.h"
#include "perft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datagen.h"

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

int MatchPattern(char* string, const char* pattern, int *patlength) {
    int matched_pattern = -1;
    const size_t patlen = strlen(pattern);
    if (strncmp(string, pattern, patlen) == 0) {
        matched_pattern = 0;
        *patlength = (int)patlen;
    }
    return matched_pattern;
}

void ParseMoves(Board *board, char* moves){
    while (1){
        moves++;
        char* move = malloc(4 + 1);

        for (int i = 0; i < 4; i++){
            move[i] = moves[0];
            moves++;
        }
        move[4] = '\0';
        if (moves[0] == 'q' || moves[0] == 'r' || moves[0] == 'b' || moves[0] == 'n'){

            move = realloc(move, 6);
            move[4] = moves[0];
            moves++;
            move[5] = '\0';
        }
        MakeMove(board, StringToMove(move, board));
        free(move);
        if (moves[0] == '\n') break;
    }
}

void SetPosition(char* line, Board *board) {
    const char* position_types[] = {
        "fen",
        "startpos"
    };

    int num;
    const int pos_type = MatchPatterns(line, position_types, 2, &num);
    line += num + 1;
    if (pos_type == -1) {
        printf("Invalid position type\n");
    }else if (pos_type == 0) {

        // Extract fen
        int empty_encounters = 0;
        int fen_length = 0;
        char* copy = line;
        while (copy[0] != '\0'){
            if (copy[0] == ' ') empty_encounters++;
            fen_length++;
            copy++;
            if (empty_encounters == 6) break;
        }

        *board = BoardConstructor(line);
        line += fen_length + 1;
    }else if (pos_type == 1) {
        *board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    const int parse_moves = MatchPattern(line, "moves", &num);
    line += num;

    if (parse_moves != -1){
        ParseMoves(board, line);
    }
}

void GoCommand(char* line, Board *board) {
    const char* go_types[] = {
        "movetime",
        "nodes",
        "depth",
        "wtime", // Assumes btime winc and binc follow
        "perft"
    };
    int patlength;
    int go_type = MatchPatterns(line, go_types, 5, &patlength);
    line += 1 + patlength;

    Stack stack = {
            .nodes = 0,
            .node_limit = INT_MAX,
            .print_info = true,
            .depth_limit = 255,
            .soft_node_limit = INT_MAX,
            .time_limit = INT_MAX,
            .hash_index = 0
    };

    switch (go_type) {
        case -1:
            // Go infinite
            break;
        case 0:
            int time;
            sscanf(line, "%d", &time);
            stack.time_limit = time;
            search(board, &stack);
            break;
        case 1:
            int nodes;
            sscanf(line, "%d", &nodes);
            stack.node_limit = nodes;
            search(board, &stack);
            break;
        case 2:
            int depth;
            sscanf(line, "%d", &depth);
            stack.depth_limit = depth;
            search(board, &stack);
            break;
        case 3:
            int white_time;
            int black_time;
            int white_inc;
            int black_inc;
            sscanf(line, "%d", &white_time);
            while (line[0] != ' ') line++;
            line += 2 + 5;
            sscanf(line, "%d", &black_time);
            while (line[0] != ' ') line++;
            line += 2 + 4;
            sscanf(line, "%d", &white_inc);
            while (line[0] != ' ') line++;
            line += 2 + 4;
            sscanf(line, "%d", &black_inc);

            int time_left = board->white_to_move ? white_time : black_time;
            int increment = board->white_to_move ? white_inc : black_inc;

            int time_limit = time_left / 20 + increment / 2;
            stack.time_limit = time_limit;
            search(board, &stack);
            break;
        case 4:
            int perft_depth;
            sscanf(line, "%d", &perft_depth);
            splitPerft(board, perft_depth);
            break;
    }
}

void RunDatagen(char* line){
    int num_threads;
    line += strlen("threads ");
    sscanf(line, "%d", &num_threads);

    line += num_threads > 9 ? 3 : 2;
    line += strlen("output ");
    line[strlen(line) - 1] = '\0';

    FILE *file = fopen(line, "ab");

    Datagen(file, num_threads);
}

void ReceiveCommand(char* line, Board *board) {
    const char* commands[] = {
        "position",
        "go",
        "isready",
        "quit",
        "ucinewgame",
        "uci",
        "datagen",
        "d",
    };

    int patlength = 0;
    const int matched_pattern = MatchPatterns(line, commands, 8, &patlength);
    line += patlength + 1;
    switch (matched_pattern) {
        case -1:
            printf("Invalid command\n");
            return;
        case 0:
            SetPosition(line, board);
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
            *board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            break;
        case 5:
            printf("id name Cairn\noption name Hash type spin default 16 min 1 max 33554432\nuciok\n");
            break;
        case 6:
            RunDatagen(line);
            break;
        case 7:
            PrintBoard(board);
            break;
    }
}

