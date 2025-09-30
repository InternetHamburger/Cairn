#include "perft.h"
#include "utility.h"

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "board.h"
#include "move.h"
#include "moveGeneration.h"

unsigned long long perft(Board *board, const int depth) {
    if (depth == 0) return 1;

    unsigned long long nodes = 0;

    int num_moves = 0;
    Move moves[256];
    GetMoves(board, moves, &num_moves);
    const Board copy = *board;
    for (int i = 0; i < num_moves; i++) {

        if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
            continue;
        }
        MakeMove(board, moves[i]);
        if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            *board = copy;
            continue;
        }
        const unsigned long long u = perft(board, depth - 1);
        nodes += u;
        *board = copy;
    }

    return nodes;
}

unsigned long long splitPerft(Board *board, const int depth) {
    unsigned long long nodes = 0;

    int num_moves = 0;
    Move moves[256];
    GetMoves(board, moves, &num_moves);
    const Board copy = *board;
    double start = clock();
    for (int i = 0; i < num_moves; i++) {

        if (GetFlag(moves[i]) == Castle && !IsLegalCastle(board, moves[i])){
            continue;
        }
        MakeMove(board, moves[i]);
        if (IsAttackedBySideToMove(board, board->white_to_move, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            *board = copy;
            continue;
        }
        const uint64_t u = perft(board, depth - 1);
        printf("%s: %llu\n", MoveToString(moves[i]), u);
        nodes += u;
        *board = copy;
    }
    double elapsed = clock() - start;
    printf("\nNodes searched: %llu\n", nodes);
    printf("Knps: %lf\n", round(nodes / elapsed));
    return nodes;
}