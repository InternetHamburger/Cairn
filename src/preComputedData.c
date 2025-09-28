#include "preComputedData.h"

#include <stdio.h>
#include <stdint.h>

unsigned long long knight_moves[64];
unsigned long long king_moves[64];

uint64_t a_file = 0x0101010101010101;
uint64_t first_rank = 0xff00000000000000;

__attribute__((constructor))  // runs before main()
static void init_table(void) {
    // right-down, right-up, up-right, up-left, left-up, left-down, down-left, down-right
    const int knight_rank_changes[] = {1, -1, -2, -2, -1, 1, 2, 2};
    const int knight_file_changes[] = {2, 2, 1, -1, -2, -2, -1, 1};

    // Right, left, up, down, right-up, left-up, right-down, left-down
    const int king_rank_changes[] = {0, 0, -1, 1, -1, -1, 1, 1};
    const int king_file_changes[] = {1, -1, 0, 0, -1, 1, -1, 1};

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
    }
}