#ifndef CAIRN_SEARCH_H
#define CAIRN_SEARCH_H

#include "board.h"

#define NEG_INF (-32767)
#define CHECKMATE (-32567)

typedef struct {
    uint64_t nodes;
    double start_time;
    uint64_t time_limit;
    uint64_t node_limit;
    int depth_limit;
    uint64_t soft_node_limit;
    bool print_info;
    uint64_t hashes[MAX_NUM_PLY]; // For threefold repetition
    int hash_index;
    Move pv_table[257][218];
} Stack;

typedef struct {
    int length, score;
    Move line[256];
} PVariation;

typedef struct{
    Move best_move;
    int score;
    uint64_t nodes;
    int depth;
} SearchResult;


SearchResult search(Board *board, Stack *stack);

#endif //CAIRN_SEARCH_H