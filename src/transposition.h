#ifndef CAIRN_TRANSPOSITION_H
#define CAIRN_TRANSPOSITION_H

#include <stdint.h>
#include "board.h"

#define EXACT 0b00000000
#define UPPER 0b01000000
#define LOWER 0b10000000

typedef struct{
    uint64_t hash;
    Move best_move;
    int16_t score;
    uint8_t depth_node_type; // ttdddddd  t - type (0-exact, 1-upper bound, 2-lower bound)  d - depth
} Entry;


typedef struct{
    int num_entries;
    Entry* entries;
} TT;

extern TT tt;

int GetDepth(Entry entry);
int GetEntryType(Entry entry);
void ZeroTT();
#endif //CAIRN_TRANSPOSITION_H
