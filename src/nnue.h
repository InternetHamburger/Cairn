#ifndef CAIRN_NNUE_H
#define CAIRN_NNUE_H

#include "search.h"

void load_incbin();
int get_index(Piece piece, int square, bool flip, bool inverse);
int nnueval(const Board* board);
int nnue_eval(Thread* thread, const Board* board, int ply);
void init_accumulator_stack(Thread* thread, const Board* board, nnue_t* nnue);
void update_accumulators(const Board* board, Move move, nnue_t* nnue);
void update_nnue_stack(Thread* thread, Move move, int ply);

extern Parameters parameters;

#endif //CAIRN_NNUE_H