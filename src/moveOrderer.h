#ifndef CAIRN_MOVEORDERER_H
#define CAIRN_MOVEORDERER_H

#include "board.h"
#include "search.h"
#include "utility.h"

#define MAX_HISTORY 16384

void ZeroHist(Thread* thread);
void ZeroKillers(Thread* thread);
void UpdateHistTable(Thread* thread, int ply, Move move, int bonus);
void UpdateKillers(Thread* thread,  Move move, int ply);
void OrderMoves(Thread* thread, Move* moves, int move_length, int ply, Move tt_move);
void OrderCaptures(Board *board, Move* moves, int move_length);

#endif //CAIRN_MOVEORDERER_H
