#ifndef CAIRN_MOVEORDERER_H
#define CAIRN_MOVEORDERER_H

#include "board.h"
#include "utility.h"

#define MAX_HISTORY 16384

void UpdateHistTable(const Board* board, Move move, int bonus);
void OrderMoves(Board *board, Move* moves, int move_length, Move tt_move);
void ZeroHist();

#endif //CAIRN_MOVEORDERER_H
