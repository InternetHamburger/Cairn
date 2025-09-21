#ifndef CAIRN_DATAGEN_H
#define CAIRN_DATAGEN_H

#include "board.h"

typedef struct { // In viriformat: https://github.com/cosmobobak/viriformat
    // Packed board
    unsigned long long occupied;
    unsigned long long pieces_0;
    unsigned long long pieces_1;
    unsigned short stm_enPassant_hm;
    unsigned short full_move;
    unsigned short score;
    short result;

    // Combined moves and scores
    unsigned long moves[MAX_NUM_PLY]; // Longest possible game;
} Game;

typedef struct {
    Game game;
    unsigned long long thread_id;
} Thread;

Piece ConvertPiece(Piece piece);
Board GenerateRandomPosition(unsigned long long *seed);
void* PlayGame(void* arg);

#endif //CAIRN_DATAGEN_H