#include "uci.h"
#include "board.h"
#include "search.h"
#include "utility.h"
#include "transposition.h"
#include "perft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "datagen.h"
#include "evaluation.h"
#include "moveOrderer.h"
#include "moveGeneration.h"

int MatchPatterns(char* string, const char* patterns[], size_t num_patterns, int *patlength) {
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

void ParseMoves(Board *board, char* moves, Stack *stack){
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
        Move make_move = StringToMove(move, board);
        assert(make_move.value != 0);
        MakeMove(board, make_move);
        stack->hash_index++;
        stack->hashes[stack->hash_index] = board->zobrist_hash;
        free(move);
        if (moves[0] == '\n') break;
    }
}

void SetPosition(char* line, Board *board, Stack *stack) {
    const char* position_types[] = {
        "fen",
        "startpos"
    };



    int num = -1900000;
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
        line += fen_length;
    }else if (pos_type == 1) {
        *board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    stack->hash_index = 0;
    stack->hashes[stack->hash_index] = board->zobrist_hash;

    const int parse_moves = MatchPattern(line, "moves", &num);
    line += num;


    if (parse_moves != -1){
        ParseMoves(board, line, stack);
    }
}

void GoCommand(char* line, Board *board, Stack *stack) {
    const char* go_types[] = {
        "movetime",
        "nodes",
        "depth",
        "wtime", // Assumes btime winc and binc follow
        "perft",
        "softnodes",
    };
    int patlength;
    int go_type = MatchPatterns(line, go_types, 6, &patlength);
    line += 1 + patlength;


    stack->nodes = 0;
    stack->node_limit = INT_MAX;
    stack->print_info = true;
    stack->depth_limit = 255;
    stack->soft_node_limit = INT_MAX;
    stack->time_limit = INT_MAX;


    switch (go_type) {
        case -1:
            // Go infinite
            break;
        case 0:
            int time;
            sscanf(line, "%d", &time);
            stack->time_limit = time;
            search(board, stack);
            break;
        case 1:
            int nodes;
            sscanf(line, "%d", &nodes);
            stack->node_limit = nodes;
            search(board, stack);
            break;
        case 2:
            int depth;
            sscanf(line, "%d", &depth);
            stack->depth_limit = depth;
            search(board, stack);
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
            stack->time_limit = time_limit;
            search(board, stack);
            break;
        case 4:
            int perft_depth;
            sscanf(line, "%d", &perft_depth);
            splitPerft(board, perft_depth);
            break;
        case 5:
            int soft_nodes;
            sscanf(line, "%d", &soft_nodes);
            stack->soft_node_limit = soft_nodes;
            search(board, stack);
            break;
    }
}

void RunDatagen(char* line, char* this_path){
    int num_threads;
    line += strlen("threads ");
    sscanf(line, "%d", &num_threads);

    line += num_threads > 9 ? 3 : 2;

    line += strlen("seed ");
    uint64_t seed;
    sscanf(line, "%llu", &seed);
    while (1){
        if (line[0] != ' ') line++;
        else break;
    }
    line++;

    line += strlen("output ");
    line[strlen(line) - 1] = '\0';


    Datagen(line, this_path, num_threads, seed);
}

void SetOption(char* line){
    line += strlen("name ");

    const char* option_types[] = {
            "Hash",
            "Threads"
    };

    int patlength;
    int option_type = MatchPatterns(line, option_types, 6, &patlength);

    line += 1 + patlength;
    line += strlen("value ");

    switch (option_type) {
        case -1:
            printf("Invalid option type\n");
            break;
        case 0:
            uint64_t hash_size;
            sscanf(line, "%llu", &hash_size);

            free(tt.entries);
            int num_entries = (int)(hash_size * 1000000 / sizeof(Entry));
            Entry* entries = malloc(num_entries * sizeof(Entry));
            tt.num_entries = num_entries;
            tt.entries = entries;
            for (int i = 0; i < tt.num_entries; i++) {
                tt.entries[i].hash = 0;
                tt.entries[i].best_move.value = 0;
                tt.entries[i].score = 0;
                tt.entries[i].depth_node_type = 0;
            }
            break;
        case 1:
            break;
        default:
            printf("Invalid option type\n");
            break;
    }
}

void ReceiveCommand(char* line, Board *board, char* this_path, Stack *stack) {
    const char* commands[] = {
        "position",
        "go",
        "isready",
        "quit",
        "ucinewgame",
        "uci",
        "datagen",
        "d",
        "eval",
        "r",
        "setoption"
    };

    int patlength = 0;
    const int matched_pattern = MatchPatterns(line, commands, 11, &patlength);
    line += patlength + 1;
    switch (matched_pattern) {
        case -1:
            printf("Invalid command\n");
            return;
        case 0:
            SetPosition(line, board, stack);
            break;
        case 1:
            GoCommand(line, board, stack);
            break;
        case 2:
            printf("readyok\n");
            return;
        case 3:
            exit(0);
        case 4:
            ZeroTT();
            ZeroHist();
            *board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            Stack new = {
                    .nodes = 0,
                    .node_limit = INT_MAX,
                    .print_info = true,
                    .depth_limit = 255,
                    .soft_node_limit = INT_MAX,
                    .time_limit = INT_MAX,
                    .hash_index = 0
            };
            *stack = new;
            break;
        case 5:
            printf("id name Cairn\n"
                   "option name Hash type spin default 16 min 1 max 33554432\n"
                   "option name Threads type spin default 1 min 1 max 1\n"
                   "uciok\n");
            break;
        case 6:
            RunDatagen(line, this_path);
            break;
        case 7:
            PrintBoard(board);
            printf("Fen: %s\n", BoardToFen(board));
            printf("Key: %llu\n", board->zobrist_hash);
            break;
        case 8:
            printf("Raw eval: %d\n", eval(board));
            break;
        case 9:
            int margin;
            char move_str[6];
            sscanf(line, "%d %s", &margin, &move_str);
            Move move = StringToMove(move_str, board);
            printf("%s: %d\n", MoveToString(move), staticExchangeEvaluation(board, move, margin));
            break;
        case 10:
            SetOption(line);
            break;
    }
}