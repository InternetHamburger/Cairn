#ifndef CAIRN_DATAGEN_H
#define CAIRN_DATAGEN_H

#include "board.h"
#include <stdio.h>

typedef struct { // In viriformat: https://github.com/cosmobobak/viriformat
    // Packed board
    unsigned long long occupied;
    unsigned long long pieces_0;
    unsigned long long pieces_1;
    unsigned long long pieces_2;
    unsigned long long pieces_3;
    unsigned short stm_enPassant_hm;
    unsigned short full_move;
    unsigned short score;
    short result;

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
void Datagen(FILE *file, int num_threads);

#endif //CAIRN_DATAGEN_H