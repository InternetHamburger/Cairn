#ifndef CAIRN_PERFT_H
#define CAIRN_PERFT_H
#include "board.h"

uint64_t perft(Board *board, int depth);
uint64_t splitPerft(Board *board, int depth);

#endif //CAIRN_PERFT_H