#include "zobrist.h"

unsigned long long zobrist_squares[64][BlackKing + 1];
unsigned long long zobrist_ep_squares[64];
unsigned long long zobrist_stm;
unsigned long long zobrist_white_kingside;
unsigned long long zobrist_white_queenside;
unsigned long long zobrist_black_kingside;
unsigned long long zobrist_black_queenside;

__attribute__((constructor))
void Init_tables(){
    unsigned long long seed = 13738256983174657986ULL;
    for (int i = 0; i < 64; i++){
        for (int j = 0; j < 15; j++) {
            zobrist_squares[i][j] = PseudorandomNumber(&seed);
        }
        zobrist_ep_squares[i] = PseudorandomNumber(&seed);
    }
    zobrist_stm = PseudorandomNumber(&seed);
    zobrist_white_kingside = PseudorandomNumber(&seed);
    zobrist_white_queenside = PseudorandomNumber(&seed);
    zobrist_black_kingside = PseudorandomNumber(&seed);
    zobrist_black_queenside = PseudorandomNumber(&seed);
}

unsigned long long PseudorandomNumber(unsigned long long *seed) {
    *seed ^= *seed >> 12;
    *seed ^= *seed << 25;
    *seed ^= *seed >> 27;
    *seed *= 0x2545F4914F6CDD1DULL;
    return *seed;
}