#include "preComputedData.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

unsigned long long knight_moves[64];
unsigned long long king_moves[64];

const uint64_t a_file = 0x0101010101010101;
const uint64_t first_rank = 0xff00000000000000;

uint64_t rays[64][8];
uint64_t rank_attacks[256][8];
smsk masks;

uint64_t diagonalMask(int sq) {
    const uint64_t maindia = 0x8040201008040201;
    int diag  = (sq&7) - (sq>>3);
    return diag >= 0 ? maindia >> diag*8 : maindia << -diag*8;
}

uint64_t antiDiagMask(int sq) {
    const uint64_t maindia = 0x0102040810204080;
    int diag  = 7 - (sq&7) - (sq>>3);
    return diag >= 0 ? maindia >> diag*8 : maindia << -diag*8;
}

uint64_t diagonalAttacks(uint64_t occ, int sq) {
    uint64_t forward, reverse;
    forward = occ & masks[sq].diagonalMaskEx;
    reverse  = _byteswap_uint64(forward);
    forward -= masks[sq].bitMask;
    reverse -= _byteswap_uint64(masks[sq].bitMask);
    forward ^= _byteswap_uint64(reverse);
    forward &= masks[sq].diagonalMaskEx;
    return forward;
}

uint64_t antiDiagAttacks(uint64_t occ, int sq) {
    uint64_t forward, reverse;
    forward  = occ & masks[sq].antidiagMaskEx;
    reverse  = _byteswap_uint64(forward);
    forward -= masks[sq].bitMask;
    reverse -= _byteswap_uint64(masks[sq].bitMask);
    forward ^= _byteswap_uint64(reverse);
    forward &= masks[sq].antidiagMaskEx;
    return forward;
}

uint64_t fileAttacks(uint64_t occ, int sq) {
    uint64_t forward, reverse;
    forward  = occ & masks[sq].fileMaskEx;
    reverse  = _byteswap_uint64(forward);
    forward -= masks[sq].bitMask;
    reverse -= _byteswap_uint64(masks[sq].bitMask);
    forward ^= _byteswap_uint64(reverse);
    forward &= masks[sq].fileMaskEx;
    return forward;
}

uint64_t rankAttacks(uint64_t occ, int sq)
{
    return rank_attacks[occ >> (8 * (sq >> 3)) & 0b11111111][sq & 7] << (8 * (sq >> 3));
}

uint64_t allAttacks(uint64_t occ, int sq)
{
    return diagonalAttacks(occ, sq) +
           antiDiagAttacks(occ, sq) +
           fileAttacks(occ, sq) +
           rankAttacks(occ, sq);
}

__attribute__((constructor))  // runs before main()
static void init_table(void) {
    // right-down, right-up, up-right, up-left, left-up, left-down, down-left, down-right
    const int knight_rank_changes[] = {1, -1, -2, -2, -1, 1, 2, 2};
    const int knight_file_changes[] = {2, 2, 1, -1, -2, -2, -1, 1};

    // Right, left, up, down, right-up, left-up, right-down, left-down
    const int king_rank_changes[] = {0, 0, -1, 1, -1, -1, 1, 1};
    const int king_file_changes[] = {1, -1, 0, 0, -1, 1, -1, 1};

    // right-up, left-up, right-down, left-down, right, left, up, down
    const int rank_directions[] = {-1, -1, 1, 1, 0, 0, -1, 1};
    const int file_directions[] = {-1, 1, -1, 1, 1, -1, 0, 0};

    uint64_t files[8] = {0};

    for (int i = 0; i < 8; i++)
    {
        files[i] = a_file << i;
    }

    for (int i = 0; i < 64; i++)
    {
        masks[i].bitMask = 1ULL << i;
        masks[i].fileMaskEx = files[i % 8] - (1ULL << i);
        masks[i].antidiagMaskEx = antiDiagMask(i) - (1ULL << i);
        masks[i].diagonalMaskEx = diagonalMask(i) - (1ULL << i);
    }

    for (uint32_t i = 0; i < 256; i++)
    {
        for (int sq = 0; sq < 8; sq++)
        {
            int file = sq;
            uint64_t attacks = 0;
            while (1)
            {
                file++;
                if (file >= 8)
                    break;

                attacks |= 1ULL << file;

                if (1ULL << file & i)
                    break;
            }

            file = sq;
            while (1)
            {
                file--;
                if (file < 0)
                    break;

                attacks |= 1ULL << file;

                if (1ULL << file & i)
                    break;
            }

            rank_attacks[i][sq] = attacks;
        }
    }

    for (int i = 0; i < 64; i++) {
        const int file = i % 8;
        const int rank = i / 8;

        unsigned long long knight_bitboard = 0;
        unsigned long long king_bitboard = 0;
        for (int direction = 0; direction < 8; direction++){
            int new_file = file + knight_file_changes[direction];
            int new_rank = rank + knight_rank_changes[direction];
            if (!(new_rank >= 8 || new_rank < 0 || new_file >= 8 || new_file < 0)){
                int target_square = new_rank * 8 + new_file;

                knight_bitboard |= 1ULL << target_square;
            }


            new_file = file + king_file_changes[direction];
            new_rank = rank + king_rank_changes[direction];
            if (!(new_rank >= 8 || new_rank < 0 || new_file >= 8 || new_file < 0)){
                int target_square = new_rank * 8 + new_file;

                king_bitboard |= 1ULL << target_square;
            }
        }
        knight_moves[i] = knight_bitboard;
        king_moves[i] = king_bitboard;

        for (int direction = 0; direction < 8; direction++){
            uint64_t ray = 0;
            int curr_rank = i / 8;
            int curr_file = i % 8;
            while (1){
                curr_rank += rank_directions[direction];
                curr_file += file_directions[direction];

                int target_square = 8 * curr_rank + curr_file;

                if (!(curr_rank >= 0 && curr_rank < 8 && curr_file >= 0 && curr_file < 8)){
                    rays[i][direction] = ray;
                    break; // Outside board
                }
                ray |= 1ULL << target_square;
            }
        }
    }
}