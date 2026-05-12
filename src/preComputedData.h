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

typedef struct{
    uint64_t mask;
    uint64_t magic;
    uint8_t shift;
} magic_entry;

uint64_t diagonalAttacks(uint64_t occ, int sq);
uint64_t antiDiagAttacks(uint64_t occ, int sq);
uint64_t fileAttacks(uint64_t occ, int sq);
uint64_t rankAttacks(uint64_t occ, int sq);
uint64_t allAttacks(uint64_t occ, int sq);
uint64_t project_bits(uint64_t mask, uint64_t bits);

uint64_t rook_attack(uint64_t occ, int sq);
uint64_t bishop_attack(uint64_t occ, int sq);

extern uint64_t knight_moves[TABLE_SIZE];
extern uint64_t king_moves[TABLE_SIZE];
extern const uint64_t a_file;
extern const uint64_t first_rank;
extern uint64_t rays[TABLE_SIZE][8];
extern smsk masks;
extern uint64_t rook_table[64][4096];
extern magic_entry rook_magics[64];
extern magic_entry bishop_magics[64];

#endif //CAIRN_PRECOMPUTEDDATA_H
