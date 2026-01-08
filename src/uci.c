#include "uci.h"
#include "board.h"
#include "perft.h"
#include "search.h"
#include "utility.h"
#include "datagen.h"
#include "evaluation.h"
#include "moveOrderer.h"
#include "transposition.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void ParseMoves(Board *board, char* moves, Stack *stack){
    char* token;
    const char delimiter[] = " ";
    token = strtok(moves, delimiter);

    while (token != NULL){

        Move make_move = StringToMove(token, board);
        assert(make_move.value != 0);
        MakeMove(board, make_move);

        stack->hash_index++;
        stack->hashes[stack->hash_index] = board->zobrist_hash;

        token = strtok(NULL, delimiter);
    }
}

void SetPosition(char* line, Board *board, Stack *stack) {
    char* token;
    const char delimiter[] = " ";
    token = strtok(line, delimiter);

    if (strncmp(token, "fen", 3) == 0){
        line += 4;
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
    }
    else if (strncmp(token, "startpos", 8) == 0){
        line += 9;
        *board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    else{
        printf("Invalid position type\n");
        return;
    }

    stack->hash_index = 0;
    stack->hashes[stack->hash_index] = board->zobrist_hash;

    token = strtok(line, delimiter);
    if (token != NULL && strncmp(token, "moves", 5) == 0){
        line += 6;
        ParseMoves(board, line, stack);
    }
}

void GoCommand(char* line, Board *board, Stack *stack) {
    stack->nodes = 0;
    stack->node_limit = INT_MAX;
    stack->print_info = true;
    stack->depth_limit = 255;
    stack->soft_node_limit = INT_MAX;
    stack->time_limit = INT_MAX;

    int movetime = INT_MAX;
    int nodes = INT_MAX;
    int softnodes = INT_MAX;
    int depth = 255;
    int wtime = 0;
    int btime = 0;
    int winc = 0;
    int binc = 0;

    char* token;
    const char delimiter[] = " ";

    token = strtok(line, delimiter);
    while (token){
        if (strncmp(token, "movetime", 8) == 0){
            token = strtok(NULL, delimiter);
            sscanf(token, "%d", &movetime);
        }
        else if (strncmp(token, "nodes", 5) == 0){
            token = strtok(NULL, delimiter);
            sscanf(token, "%d", &nodes);
        }
        else if (strncmp(token, "softnodes", 9) == 0){
            token = strtok(NULL, delimiter);
            sscanf(token, "%d", &softnodes);
        }
        else if (strncmp(token, "depth", 5) == 0){
            token = strtok(NULL, delimiter);
            sscanf(token, "%d", &depth);
        }
        else if (strncmp(token, "wtime", 5) == 0){
            token = strtok(NULL, delimiter);
            sscanf(token, "%d", &wtime);
        }
        else if (strncmp(token, "btime", 5) == 0){
            token = strtok(NULL, delimiter);
            sscanf(token, "%d", &btime);
        }
        else if (strncmp(token, "winc", 4) == 0){
            token = strtok(NULL, delimiter);
            sscanf(token, "%d", &winc);
        }
        else if (strncmp(token, "binc", 4) == 0){
            token = strtok(NULL, delimiter);
            sscanf(token, "%d", &binc);
        }
        else if (strncmp(token, "perft", 5) == 0){
            token = strtok(NULL, delimiter);
            int perft_depth;
            sscanf(token, "%d", &perft_depth);
            splitPerft(board, perft_depth);
            return;
        }
        token = strtok(NULL, delimiter);
    }

    int time_left = board->white_to_move ? wtime : btime;
    int increment = board->white_to_move ? winc : binc;

    int time_limit = time_left / 20 + increment / 2;

    stack->node_limit = nodes;
    stack->print_info = true;
    stack->depth_limit = depth;
    stack->soft_node_limit = softnodes;
    stack->time_limit = __min(time_limit, movetime);
    search(board, stack);
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

    char* token;
    const char delimiter[] = " ";
    token = strtok(line, delimiter);

    if (strncmp(token, "Hash", 4) == 0){
        token = strtok(NULL, delimiter);
        token = strtok(NULL, delimiter);
        uint64_t hash_size;
        sscanf(token, "%llu", &hash_size);
        free(tt.entries);
        int num_entries = (int)(hash_size * 1000000 / sizeof(Entry));
        Entry* entries = malloc(num_entries * sizeof(Entry));
        memset(entries, 0, num_entries * sizeof(Entry));
        tt.num_entries = num_entries;
        tt.entries = entries;
    }
    else if (strncmp(token, "Threads", 7) == 0){

    }
    else{
        printf("Invalid option type\n");
    }
}

void ReceiveCommand(char* line, Board *board, char* this_path, Stack *stack) {
    char* token;
    const char delimiter[] = " ";
    token = strtok(line, delimiter);

    if (strncmp(token, "position", 8) == 0){
        line += 9;
        SetPosition(line, board, stack);
    }
    else if (strncmp(token, "go", 2) == 0){
        line += 3;
        GoCommand(line, board, stack);
    }
    else if (strncmp(token, "isready", 7) == 0){
        printf("readyok\n");
    }
    else if (strncmp(token, "quit", 4) == 0){
        exit(0);
    }
    else if (strncmp(token, "ucinewgame", 10) == 0){
        ZeroTT();
        ZeroHist();
        *board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        Stack new = {
                .nodes = 0,
                .node_limit = INT64_MAX,
                .print_info = true,
                .depth_limit = 255,
                .soft_node_limit = INT64_MAX,
                .time_limit = INT64_MAX,
                .hash_index = 0
        };
        *stack = new;
    }
    else if (strncmp(token, "uci", 3) == 0){
        printf("id name Cairn\nid author InternetHamburger\n\n"
               "option name Hash type spin default 16 min 1 max 33554432\n"
               "option name Threads type spin default 1 min 1 max 1\n"
               "uciok\n");
    }
    else if (strncmp(token, "datagen", 7) == 0){
        line += 8;
        RunDatagen(line, this_path);
    }
    else if (strncmp(token, "d", 1) == 0){
        PrintBoard(board);
        printf("Fen: %s\n", BoardToFen(board));
        printf("Key: %llu\n", board->zobrist_hash);
    }
    else if (strncmp(token, "eval", 4) == 0){
        printf("Raw eval: %d\n", eval(board));
    }
    else if (strncmp(token, "setoption", 9) == 0){
        line += 10;
        SetOption(line);
    }
    else{
        printf("Invalid command\n");
    }
}