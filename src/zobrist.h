#ifndef CAIRN_ZOBRIST_H
#define CAIRN_ZOBRIST_H

#include "board.h"

extern uint64_t zobrist_squares[64][BlackKing + 1];
extern uint64_t zobrist_ep_squares[64];
extern uint64_t zobrist_stm;
extern uint64_t zobrist_white_kingside;
extern uint64_t zobrist_white_queenside;
extern uint64_t zobrist_black_kingside;
extern uint64_t zobrist_black_queenside;

uint64_t PseudorandomNumber(uint64_t *seed);

#endif //CAIRN_ZOBRIST_H
