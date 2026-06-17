#ifndef CAIRN_NNUE_H
#define CAIRN_NNUE_H

#include <stdint.h>
#include "board.h"

#define INPUT_SIZE 768
#define HL_SIZE 64

#define QA 255
#define QB 64

#define EVAL_SCALE 400

typedef struct{
    alignas(64) int16_t feature_weights[INPUT_SIZE * HL_SIZE];
    alignas(64) int16_t feature_bias[HL_SIZE];
    alignas(64) int16_t out_weights[2 * HL_SIZE];
    alignas(64) int16_t out_bias;
} Parameters;

typedef struct{
    alignas(64) int32_t white_accumulator[HL_SIZE];
    alignas(64) int32_t black_accumulator[HL_SIZE];
} nnue_t;

void load_incbin();
int get_index(Piece piece, int square, bool inverse);
int nnueval(const Board* board);
int nnue_eval(const Board* board, nnue_t* nnue);
void init_accumulators(const Board* board, nnue_t* nnue);
void update_accumulators(const Board* board, Move move, nnue_t* nnue);

extern Parameters parameters;

#endif //CAIRN_NNUE_H