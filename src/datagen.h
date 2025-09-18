#ifndef CAIRN_DATAGEN_H
#define CAIRN_DATAGEN_H

typedef struct { // In viriformat: https://github.com/cosmobobak/viriformat
    // Packed board
    unsigned long long occupied;
    unsigned long long pieces_0;
    unsigned long long pieces_1;
    short stm_enPassant_hm;
    int fm_score;
    short result;

    // Combined moves and scores
    unsigned long moves[17697]; // Longest possible game;
} Game;

typedef struct {
    Game game;
    unsigned long long thread_id;
} Thread;

#endif //CAIRN_DATAGEN_H