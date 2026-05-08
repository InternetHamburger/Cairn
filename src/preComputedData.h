#ifndef CAIRN_PRECOMPUTEDDATA_H
#define CAIRN_PRECOMPUTEDDATA_H

#include <stdint.h>

#define TABLE_SIZE 64

typedef struct
{
    uint64_t bitMask;
    uint64_t diagonalMaskEx;
    uint64_t antidiagMaskEx;
    uint64_t fileMaskEx;
} smsk[64];

uint64_t diagonalAttacks(uint64_t occ, int sq);
uint64_t antiDiagAttacks(uint64_t occ, int sq);
uint64_t fileAttacks(uint64_t occ, int sq);
uint64_t allAttacks(uint64_t occ, int sq);

extern uint64_t knight_moves[TABLE_SIZE];
extern uint64_t king_moves[TABLE_SIZE];
extern const uint64_t a_file;
extern const uint64_t first_rank;
extern uint64_t rays[TABLE_SIZE][8];
extern smsk masks;

#endif //CAIRN_PRECOMPUTEDDATA_H
