#include "nnue.h"
#include "utility.h"
#include "incbin/incbin.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CLAMP(x, a, b) __min(__max(x, a), b)

INCBIN(quant, "C:/c/beans.bin");

Parameters parameters;

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