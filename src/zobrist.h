#ifndef CAIRN_ZOBRIST_H
#define CAIRN_ZOBRIST_H

#include "board.h"

extern unsigned long long zobrist_squares[64][BlackKing + 1];
extern unsigned long long zobrist_ep_squares[64];
extern unsigned long long zobrist_stm;
extern unsigned long long zobrist_white_kingside;
extern unsigned long long zobrist_white_queenside;
extern unsigned long long zobrist_black_kingside;
extern unsigned long long zobrist_black_queenside;

unsigned long long PseudorandomNumber(unsigned long long *seed);

#endif //CAIRN_ZOBRIST_H
