#ifndef CAIRN_MOVEORDERER_H
#define CAIRN_MOVEORDERER_H

#include "board.h"
#include "utility.h"

#define MAX_HISTORY 16384

void UpdateHistTable(const Board* board, Move move, int bonus);
void UpdateKillers(Move move, int ply);
void OrderMoves(Board *board, Move* moves, int move_length, int ply, Move tt_move);
void OrderCaptures(Board *board, Move* moves, int move_length);
void ZeroHist();

#endif //CAIRN_MOVEORDERER_H
