#ifndef CAIRN_NNUE_H
#define CAIRN_NNUE_H

#include <stdint.h>
#include "board.h"

#define INPUT_SIZE 768
#define HL_SIZE 32

#define QA 255
#define QB 64

#define EVAL_SCALE 400

typedef struct{
    int16_t feature_weights[INPUT_SIZE * HL_SIZE];
    int16_t feature_bias[HL_SIZE];
    int16_t out_weights[2 * HL_SIZE];
    int16_t out_bias;
} Parameters;

void load_incbin();
int nnueval(const Board* board);

#endif //CAIRN_NNUE_H