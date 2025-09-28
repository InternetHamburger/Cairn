#ifndef CAIRN_PRECOMPUTEDDATA_H
#define CAIRN_PRECOMPUTEDDATA_H

#include <stdint.h>

#define TABLE_SIZE 64
extern unsigned long long knight_moves[TABLE_SIZE];
extern unsigned long long king_moves[TABLE_SIZE];
extern const uint64_t a_file;
extern const uint64_t first_rank;
extern uint64_t rays[TABLE_SIZE][8];

#endif //CAIRN_PRECOMPUTEDDATA_H
