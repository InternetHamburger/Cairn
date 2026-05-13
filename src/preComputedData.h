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

typedef struct
{
    uint64_t mask;
    uint64_t magic;
    uint8_t shift;
    uint64_t* table;
    uint64_t table_size;
} magic_entry;

typedef struct
{
    uint64_t magic;
    uint64_t table_size;
} magic_number;

uint64_t rook_attack(uint64_t occ, int sq);
uint64_t bishop_attack(uint64_t occ, int sq);

extern uint64_t knight_moves[TABLE_SIZE];
extern uint64_t king_moves[TABLE_SIZE];
extern const uint64_t a_file;
extern const uint64_t first_rank;

#endif //CAIRN_PRECOMPUTEDDATA_H
