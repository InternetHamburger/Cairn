#ifndef CAIRN_PRECOMPUTEDDATA_H
#define CAIRN_PRECOMPUTEDDATA_H

#include <stdint.h>

#define TABLE_SIZE 64
extern uint64_t knight_moves[TABLE_SIZE];
extern uint64_t king_moves[TABLE_SIZE];
extern const uint64_t a_file;
extern const uint64_t first_rank;
extern uint64_t rays[TABLE_SIZE][8];

#endif //CAIRN_PRECOMPUTEDDATA_H
