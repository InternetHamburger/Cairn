#ifndef CAIRN_DATAGEN_H
#define CAIRN_DATAGEN_H

#include "board.h"
#include <stdio.h>
#include <stdint.h>

typedef struct { // In viriformat: https://github.com/cosmobobak/viriformat
    // Packed board
    unsigned long long occupied;
    uint8_t pieces[16];
    uint8_t stm_enPassant;
    uint8_t half_move;
    uint16_t full_move;
    int16_t score;
    uint8_t result;

    // NOTE: DO NOT WRITE TO GAME FILE
    int ply;

    // Combined moves and scores
    unsigned long moves[MAX_NUM_PLY]; // Longest possible game;
} Game;

typedef struct {
    Game game;
    unsigned long long thread_id;
    FILE *file;
} Thread;

Piece ConvertPiece(Piece piece);
Board GenerateRandomPosition(unsigned long long *seed);
void Datagen(char* file_path, char* this_path, int num_threads, int seed);
void* GameLoop(Thread *this);

#endif //CAIRN_DATAGEN_H