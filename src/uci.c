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

void ParseMoves(char* moves, Thread *thread){
    char* token;
    const char delimiter[] = " ";
    token = strtok(moves, delimiter);

    while (token != NULL){

        Move make_move = StringToMove(token, &thread->board);
        assert(make_move.value != 0);
        MakeMove(&thread->board, make_move);

        thread->hashes[thread->board.game_ply] = thread->board.zobrist_hash;

        token = strtok(nullptr, delimiter);
    }
}

void SetPosition(char* line, Thread *thread) {
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

        thread->board = BoardConstructor(line);
        line += fen_length;
    }
    else if (strncmp(token, "startpos", 8) == 0){
        line += 9;
        thread->board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    else{
        printf("Invalid position type\n");
        return;
    }

    thread->hashes[0] = thread->board.zobrist_hash;

    token = strtok(line, delimiter);
    if (token != NULL && strncmp(token, "moves", 5) == 0){
        line += 6;
        ParseMoves(line, thread);
    }
}

void GoCommand(char* line, Thread *thread) {
    thread->nodes = 0;
    thread->node_limit = INT_MAX;
    thread->print_info = true;
    thread->depth_limit = 255;
    thread->soft_node_limit = INT_MAX;
    thread->time_limit = INT_MAX;

    int movetime = INT_MAX;
    int nodes = INT_MAX;
    int softnodes = INT_MAX;
    int depth = 255;
    int wtime = INT_MAX;
    int btime = INT_MAX;
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
            splitPerft(&thread->board, perft_depth);
            return;
        }
        token = strtok(NULL, delimiter);
    }

    int time_left = thread->board.white_to_move ? wtime : btime;
    int increment = thread->board.white_to_move ? winc : binc;

    int time_limit = time_left / 20 + increment / 2;

    thread->node_limit = nodes;
    thread->print_info = true;
    thread->depth_limit = depth;
    thread->soft_node_limit = softnodes;
    thread->time_limit = __min(time_limit, movetime);
    search(thread);
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

void SetOption(char* line, Thread* thread){
    line += strlen("name ");

    char* token;
    const char delimiter[] = " ";
    token = strtok(line, delimiter);

    if (strncmp(token, "Hash", 4) == 0){
        token = strtok(NULL, delimiter);
        token = strtok(NULL, delimiter);
        uint64_t hash_size;
        sscanf(token, "%llu", &hash_size);
        free(thread->tt.entries);
        int num_entries = (int)(hash_size * 1000000 / sizeof(Entry));
        Entry* entries = malloc(num_entries * sizeof(Entry));
        memset(entries, 0, num_entries * sizeof(Entry));
        thread->tt.num_entries = num_entries;
        thread->tt.entries = entries;
    }
    else if (strncmp(token, "Threads", 7) == 0){

    }
    else{
        printf("Invalid option type\n");
    }
}

void ReceiveCommand(char* line, char* this_path, Thread *thread) {
    char* token;
    const char delimiter[] = " ";
    token = strtok(line, delimiter);

    if (strncmp(token, "position", 8) == 0){
        line += 9;
        SetPosition(line, thread);
    }
    else if (strncmp(token, "go", 2) == 0){
        line += 3;
        GoCommand(line, thread);
    }
    else if (strncmp(token, "isready", 7) == 0){
        printf("readyok\n");
    }
    else if (strncmp(token, "quit", 4) == 0){
        exit(0);
    }
    else if (strncmp(token, "ucinewgame", 10) == 0){
        ZeroTT(&thread->tt);
        ZeroHist(thread);
        ZeroKillers(thread);
        Thread new = {
                .nodes = 0,
                .node_limit = INT64_MAX,
                .print_info = true,
                .depth_limit = 255,
                .soft_node_limit = INT64_MAX,
                .time_limit = INT64_MAX,
                .board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
                .ss = {0}
        };
        *thread = new;
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
        PrintBoard(&thread->board);
        printf("Fen: %s\n", BoardToFen(&thread->board));
        printf("Key: %llu\n", thread->board.zobrist_hash);
    }
    else if (strncmp(token, "eval", 4) == 0){
        printf("Raw eval: %d\n", eval(&thread->board));
    }
    else if (strncmp(token, "setoption", 9) == 0){
        line += 10;
        SetOption(line, thread);
    }
    else{
        printf("Invalid command\n");
    }
}