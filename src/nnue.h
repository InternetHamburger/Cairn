#ifndef CAIRN_NNUE_H
#define CAIRN_NNUE_H

#include <stdint.h>
#include "board.h"

#define INPUT_SIZE 768
#define HL_SIZE 1024
#define NUM_OUTPUT_BUCKETS 8

#define BUCKET_DIVISOR (32 / NUM_OUTPUT_BUCKETS)

#define QA 255
#define QB 64

#define EVAL_SCALE 400

#if (__AVX512F__)
    #define FULL_VECTOR_SIZE 64
    #define HALF_VECTOR_SIZE 32
#elif (__AVX2__)
    #define FULL_VECTOR_SIZE 32
    #define HALF_VECTOR_SIZE 16
#endif

typedef int16_t vfsi16 __attribute__ ((vector_size (FULL_VECTOR_SIZE)));
typedef int16_t vhsi16 __attribute__ ((vector_size (HALF_VECTOR_SIZE)));
typedef int32_t vfsi32 __attribute__ ((vector_size (FULL_VECTOR_SIZE)));

typedef struct{
    alignas(64) int16_t feature_weights[INPUT_SIZE][HL_SIZE];
    alignas(64) int16_t feature_bias[HL_SIZE];
    alignas(64) int16_t out_weights[NUM_OUTPUT_BUCKETS][2 * HL_SIZE];
    alignas(64) int16_t out_bias[NUM_OUTPUT_BUCKETS];
} Parameters;

typedef struct{
    alignas(64) int16_t white_accumulator[HL_SIZE];
    alignas(64) int16_t black_accumulator[HL_SIZE];
} nnue_t;

void load_incbin();
int get_index(Piece piece, int square, bool inverse);
int nnueval(const Board* board);
int nnue_eval(const Board* board, nnue_t* nnue);
void init_accumulators(const Board* board, nnue_t* nnue);
void update_accumulators(const Board* board, Move move, nnue_t* nnue);

extern Parameters parameters;

#endif //CAIRN_NNUE_H