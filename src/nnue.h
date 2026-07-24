#ifndef CAIRN_NNUE_H
#define CAIRN_NNUE_H

#include "search.h"

void load_incbin();
int get_index(Piece piece, int square, bool inverse);
int nnueval(const Board* board);
int nnue_eval(const Board* board, nnue_t* nnue);
void init_accumulators(const Board* board, nnue_t* nnue);
void update_accumulators(const Board* board, Move move, nnue_t* nnue);

extern Parameters parameters;

#endif //CAIRN_NNUE_H