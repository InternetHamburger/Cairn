#include "datagen.h"
#include "board.h"
#include "utility.h"
#include "search.h"
#include "moveGeneration.h"
#include <pthread.h>
#include <stdlib.h>

void PseudorandomNumber(unsigned long long *seed) {
    *seed ^= *seed >> 12;
    *seed ^= *seed << 25;
    *seed ^= *seed >> 27;
    *seed *= 0x2545F4914F6CDD1DULL;
}

Board GenerateRandomPosition(unsigned long long *seed) {
    Board board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board prev_copy = board;
    for (int num_deep = 0; num_deep < 7; num_deep++) {
        int num_moves = 0;
        Move* moves = GetMoves(&board, &num_moves);

        int num_legal_moves = 0;
        Move* legal_moves = malloc(sizeof(Move) * num_moves);
        const Board copy = board;
        for (int i = 0; i < num_moves; i++) {
            if (GetFlag(moves[i]) == Castle && !IsLegalCastle(&board, moves[i])){
                continue;
            }
            MakeMove(&board, moves[i]);
            if (IsAttackedBySideToMove(&board, board.white_to_move, board.white_to_move ? board.black_king_square : board.white_king_square)) {
                board = copy;
                continue;
            }
            legal_moves[num_legal_moves++] = moves[i];

            board = copy;
        }
        if (num_legal_moves == 0) {
            board = prev_copy;
            num_deep--;
            PseudorandomNumber(seed);
        }
        else {
            unsigned long long rand_index = *seed % num_legal_moves;
            PseudorandomNumber(seed);

            MakeMove(&board, legal_moves[rand_index]);
        }
        prev_copy = copy;
        free(moves);
        free(legal_moves);
    }
    return board;
}

unsigned long long GetOccupied(Board *board) {
    unsigned long long occupied = 0;

    for (int i = 0; i < 64; i++) {
        if (board->squares[i]) {
            occupied |= 1ULL << FlipSquare(i);
        }
    }

    return occupied;
}

Board PrepareGame(Thread *this) {
    unsigned long long* seed = &this->thread_id;
    PseudorandomNumber(seed);

    Board rand_pos = GenerateRandomPosition(seed);
    this->game.occupied = GetOccupied(&rand_pos);

    for (int i = 0; i < 32; i++) {
        this->game.pieces_0 |= ()
    }

    return rand_pos;
}

void* PlayGame(void* arg) {
    Thread* this = (Thread*)arg;

    Board board = PrepareGame(this);



    return NULL;
}
