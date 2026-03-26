#ifndef CAIRN_DATAGEN_H
#define CAIRN_DATAGEN_H

#include "board.h"
#include <stdio.h>
#include <stdint.h>

typedef struct { // In viriformat: https://github.com/cosmobobak/viriformat
    // Packed board
    uint8_t result;

    // NOTE: DO NOT WRITE TO GAME FILE
    int ply;

    Board end_positions[1024];
    int scores[1024];
} Game;

typedef struct {
    Game game;
    unsigned long long thread_id;
    FILE *file;
} Thread;

Piece ConvertPiece(Piece piece);
Board GenerateRandomPosition(unsigned long long *seed);
void Datagen(char* file_path, char* this_path, int num_threads, uint64_t seed);
void* GameLoop(Thread *this);

#endif //CAIRN_DATAGEN_H