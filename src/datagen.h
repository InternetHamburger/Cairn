#ifndef CAIRN_DATAGEN_H
#define CAIRN_DATAGEN_H

#include "transposition.h"
#include <stdio.h>
#include <stdint.h>

typedef struct { // In viriformat: https://github.com/cosmobobak/viriformat
    // Packed board
    uint64_t occupied;
    uint8_t pieces[16];
    uint8_t stm_enPassant;
    uint8_t half_move;
    uint16_t full_move;
    int16_t score;
    uint8_t result;

    // NOTE: DO NOT WRITE TO GAME FILE
    int ply;

    // Combined moves and scores
    uint32_t moves[MAX_NUM_PLY]; // Longest possible game;
} Game;

typedef struct {
    Game game;
    uint64_t seed;
    uint64_t thread_id;
    uint64_t nodes;
    uint64_t positions;
    int games;
    FILE *file;
    Thread* thread;
} DatagenInfo;

Piece ConvertPiece(Piece piece);
void Datagen(char* file_path, int num_threads, uint64_t seed);

#endif //CAIRN_DATAGEN_H