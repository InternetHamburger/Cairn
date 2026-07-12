#include "nnue.h"
#include "utility.h"
#include "incbin/incbin.h"
#include <immintrin.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avxintrin.h>

#define CLAMP(x, a, b) __min(__max(x, a), b)

INCBIN(quant, NETWORK);

Parameters parameters;

vfsi16 vmin(vfsi16 a, vfsi16 b) {
    vfsi16 mask = a < b;          // mask: -1 (all bits set) where a < b, else 0
    return (mask & a) | (~mask & b);
}

vfsi16 vmax(vfsi16 a, vfsi16 b) {
    vfsi16 mask = a > b;
    return (mask & a) | (~mask & b);
}

#if (__AVX512F__)
static inline vfsi32 madd_epi16(vfsi16 a, vfsi16 b) {
    return (vfsi32)_mm512_madd_epi16((__m512i)a, (__m512i)b);
}
static inline vfsi16 mullo_epi16(vfsi16 a, vfsi16 b) {
    return (vfsi16)_mm512_mullo_epi16((__m512i)a, (__m512i)b);
}
#elif (__AVX2__)
static inline vfsi32 madd_epi16(vfsi16 a, vfsi16 b) {
    return (vfsi32)_mm256_madd_epi16((__m256)a, (__m256i)b);
}
static inline vfsi16 mullo_epi16(vfsi16 a, vfsi16 b) {
    return (vfsi16)_mm256_mullo_epi16((__m256i)a, (__m256i)b);
}
#endif

void load_incbin(){
    uint64_t memory_index = 0;
    memcpy(parameters.feature_weights, &gquantData[memory_index], sizeof(parameters.feature_weights));
    memory_index += sizeof(parameters.feature_weights);
    memcpy(parameters.feature_bias, &gquantData[memory_index], sizeof(parameters.feature_bias));
    memory_index += sizeof(parameters.feature_bias);

    memcpy(parameters.out_weights, &gquantData[memory_index], sizeof(parameters.out_weights));
    memory_index += sizeof(parameters.out_weights);
    memcpy(&parameters.out_bias, &gquantData[memory_index], sizeof(parameters.out_bias));
}

int piece_index(Piece piece){
    switch (piece) {
        case WhitePawn:
            return 0;
        case WhiteKnight:
            return 1;
        case WhiteBishop:
            return 2;
        case WhiteRook:
            return 3;
        case WhiteQueen:
            return 4;
        case WhiteKing:
            return 5;
        case BlackPawn:
            return 6;
        case BlackKnight:
            return 7;
        case BlackBishop:
            return 8;
        case BlackRook:
            return 9;
        case BlackQueen:
            return 10;
        case BlackKing:
            return 11;
        default:
            printf("Invalid piece");
            exit(-1);
    }
}

int get_index(Piece piece, int square, bool inverse){
    // As the weights are stored with a1=0 h8=63 and
    // my board is using a8=0 h1=63 we need to flip first
    square = FlipSquare(square);
    if (inverse){
        piece ^= 0b1000;
        square = FlipSquare(square);
    }
    return piece_index(piece) * 64 + square;
}

int nnueval(const Board* board){
    int white_acc[HL_SIZE] = {0};
    int black_acc[HL_SIZE] = {0};

    for (int square = 0; square < 64; square++){
        if (board->squares[square]){
            int index = get_index(board->squares[square], square, false);
            int flipped_index = get_index(board->squares[square], square, true);
            for (int neuron = 0; neuron < HL_SIZE; neuron++){
                white_acc[neuron] += parameters.feature_weights[index * HL_SIZE + neuron];
                black_acc[neuron] += parameters.feature_weights[flipped_index * HL_SIZE + neuron];
            }

        }
    }

    for (int neuron = 0; neuron < HL_SIZE; neuron++){
        white_acc[neuron] += parameters.feature_bias[neuron];
        black_acc[neuron] += parameters.feature_bias[neuron];
        white_acc[neuron] = CLAMP(white_acc[neuron], 0, QA) * CLAMP(white_acc[neuron], 0, QA);
        black_acc[neuron] = CLAMP(black_acc[neuron], 0, QA) * CLAMP(black_acc[neuron], 0, QA);
    }

    int* stm_acc;
    int* nstm_acc;

    if (board->white_to_move)
    {
        stm_acc = white_acc;
        nstm_acc = black_acc;
    }
    else
    {
        nstm_acc = white_acc;
        stm_acc = black_acc;
    }

    int output = 0;
    for (int neuron = 0; neuron < HL_SIZE; neuron++){
        output += stm_acc[neuron] * parameters.out_weights[neuron];
        output += nstm_acc[neuron] * parameters.out_weights[HL_SIZE + neuron];
    }
    output /= QA;
    output = (output + parameters.out_bias) * EVAL_SCALE / (QA * QB);
    return output;
}

int nnue_eval(const Board* board, nnue_t* nnue){
    int16_t* stm_acc;
    int16_t* nstm_acc;

    if (board->white_to_move)
    {
        stm_acc = nnue->white_accumulator;
        nstm_acc = nnue->black_accumulator;
    }
    else
    {
        nstm_acc = nnue->white_accumulator;
        stm_acc = nnue->black_accumulator;
    }

    vfsi16 v_zero = {0};
    vfsi16 v_qa = {0};
    v_qa = v_qa + QA;

    vfsi32 sum = {0};

    for (int neuron = 0; neuron < HL_SIZE; neuron += FULL_VECTOR_SIZE / sizeof(int16_t)){
        vfsi16 stm = *(vfsi16*)(stm_acc + neuron);
        vfsi16 nstm = *(vfsi16*)(nstm_acc + neuron);

        vfsi16 stm_weights = *(vfsi16*)(parameters.out_weights + neuron);
        vfsi16 nstm_weights = *(vfsi16*)(parameters.out_weights + HL_SIZE + neuron);

        vfsi16 stm_clamped = vmin(vmax(stm, v_zero), v_qa);
        vfsi16 nstm_clamped = vmin(vmax(nstm, v_zero), v_qa);

        vfsi32 stm_results = madd_epi16(mullo_epi16(stm_weights, stm_clamped), stm_clamped);
        vfsi32 nstm_results = madd_epi16(mullo_epi16(nstm_weights, nstm_clamped), nstm_clamped);

        sum = sum + stm_results;
        sum = sum + nstm_results;
    }
    int output = 0;
    int* arr = (int32_t*)&sum;
    for (int i = 0; i < FULL_VECTOR_SIZE / sizeof(int32_t); i++)
    {
        output += arr[i];
    }

    output /= QA;
    output = (output + parameters.out_bias) * EVAL_SCALE / (QA * QB);
    return output;
}

void init_accumulators(const Board* board, nnue_t* nnue){
    memset(nnue->white_accumulator, 0, sizeof(nnue->white_accumulator));
    memset(nnue->black_accumulator, 0, sizeof(nnue->black_accumulator));
    for (int neuron = 0; neuron < HL_SIZE; neuron++){
        nnue->white_accumulator[neuron] = parameters.feature_bias[neuron];
        nnue->black_accumulator[neuron] = parameters.feature_bias[neuron];
    }
    for (int sq = 0; sq < 64; sq++){
        if (board->squares[sq]){
            int index = get_index(board->squares[sq], sq, false);
            int flipped_index = get_index(board->squares[sq], sq, true);
            for (int neuron = 0; neuron < HL_SIZE; neuron++){
                nnue->white_accumulator[neuron] += parameters.feature_weights[index * HL_SIZE + neuron];
                nnue->black_accumulator[neuron] += parameters.feature_weights[flipped_index * HL_SIZE + neuron];
            }
        }
    }
}

void add_feature(nnue_t* nnue, Piece piece, int sq){
    int index = get_index(piece, sq, false);
    int flipped_index = get_index(piece, sq, true);
    for (int neuron = 0; neuron < HL_SIZE; neuron += FULL_VECTOR_SIZE / sizeof(int16_t)){
        vfsi16 w_acc = *(vfsi16*)(nnue->white_accumulator + neuron);
        vfsi16 b_acc = *(vfsi16*)(nnue->black_accumulator + neuron);

        vfsi16 w_weights = *(vfsi16*)(parameters.feature_weights + index * HL_SIZE + neuron);
        vfsi16 b_weights = *(vfsi16*)(parameters.feature_weights + flipped_index * HL_SIZE + neuron);

        *(vfsi16*)(nnue->white_accumulator + neuron) = w_acc + w_weights;
        *(vfsi16*)(nnue->black_accumulator + neuron) = b_acc + b_weights;
    }
}

void remove_feature(nnue_t* nnue, Piece piece, int sq){
    int index = get_index(piece, sq, false);
    int flipped_index = get_index(piece, sq, true);
    for (int neuron = 0; neuron < HL_SIZE; neuron += FULL_VECTOR_SIZE / sizeof(int16_t)){
        vfsi16 w_acc = *(vfsi16*)(nnue->white_accumulator + neuron);
        vfsi16 b_acc = *(vfsi16*)(nnue->black_accumulator + neuron);

        vfsi16 w_weights = *(vfsi16*)(parameters.feature_weights + index * HL_SIZE + neuron);
        vfsi16 b_weights = *(vfsi16*)(parameters.feature_weights + flipped_index * HL_SIZE + neuron);

        *(vfsi16*)(nnue->white_accumulator + neuron) = w_acc - w_weights;
        *(vfsi16*)(nnue->black_accumulator + neuron) = b_acc - b_weights;
    }
}

void update_accumulators(const Board* board, const Move move, nnue_t* nnue){
    const int start_square = StartSquare(move);
    const int target_square = TargetSquare(move);

    const int moved_piece = board->squares[start_square];
    const int captured_piece = board->squares[target_square];

    add_feature(nnue, moved_piece, target_square);
    remove_feature(nnue, moved_piece, start_square);

    if (captured_piece){
        remove_feature(nnue, captured_piece, target_square);
    }

    const int flag = GetFlag(move);

    if (IsPromotion(move)) {
        remove_feature(nnue, moved_piece, target_square);
        switch (flag) {
            case PromoteQueen:
                add_feature(nnue, board->white_to_move ? WhiteQueen : BlackQueen, target_square);
                break;
            case PromoteKnight:
                add_feature(nnue, board->white_to_move ? WhiteKnight : BlackKnight, target_square);
                break;
            case PromoteBishop:
                add_feature(nnue, board->white_to_move ? WhiteBishop : BlackBishop, target_square);
                break;
            case PromoteRook:
                add_feature(nnue, board->white_to_move ? WhiteRook : BlackRook, target_square);
                break;
            default:
                exit(-1);
        }
    }

    if (flag == EnPassant) {
        const bool captures_left = (target_square % 8) < (start_square % 8);
        const Piece captured_pawn = board->white_to_move ? BlackPawn : WhitePawn;
        if (captures_left) {
            const int new_square = start_square - 1;
            remove_feature(nnue, captured_pawn, new_square);
        }
        else {
            const int new_square = start_square + 1;
            remove_feature(nnue, captured_pawn, new_square);
        }
    }

    if (flag == Castle) {
        if (target_square == 62) {
            add_feature(nnue, WhiteRook, 61);
            remove_feature(nnue, WhiteRook, 63);
        }
        if (target_square == 58) {
            add_feature(nnue, WhiteRook, 59);
            remove_feature(nnue, WhiteRook, 56);
        }
        if (target_square == 6) {
            add_feature(nnue, BlackRook, 5);
            remove_feature(nnue, BlackRook, 7);
        }
        if (target_square == 2) {
            add_feature(nnue, BlackRook, 3);
            remove_feature(nnue, BlackRook, 0);
        }
    }
}