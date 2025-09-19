#ifndef CAIRN_SEARCH_H
#define CAIRN_SEARCH_H

#include "board.h"

#define NEG_INF (-32767)
#define CHECKMATE (-32567)

int search(Board *board, int depth_limit, int node_limit, int soft_node_limit, int time_limit);

typedef struct {
    unsigned long long nodes;
    double start_time;
    int time_limit;
    int node_limit;
} Stack;

#endif //CAIRN_SEARCH_H