#ifndef CAIRN_SEARCH_H
#define CAIRN_SEARCH_H

#include "board.h"

#define NEG_INF (-32767)
#define CHECKMATE (-32567)

typedef struct {
    unsigned long long nodes;
    double start_time;
    int time_limit;
    int node_limit;
    int depth_limit;
    int soft_node_limit;
    bool print_info;
    unsigned long long hashes[MAX_NUM_PLY]; // For threefold repetition
    int hash_index;
} Stack;

typedef struct{
    Move best_move;
    int score;
    double time_in_part;
} SearchResult;

SearchResult search(Board *board, Stack *stack);

#endif //CAIRN_SEARCH_H