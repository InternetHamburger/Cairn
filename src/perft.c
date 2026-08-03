#include <time.h>
#include <math.h>
#include <stdio.h>

#include "perft.h"
#include "utility.h"
#include "moveGeneration.h"

uint64_t perft(Board *board, const int depth) {
    if (depth == 0) return 1;

    uint64_t nodes = 0;
    Move moves[256];
    int num_moves = GetMoves(board, moves);
    const Board copy = *board;
    for (int i = 0; i < num_moves; i++) {
        MakeMove(board, moves[i]);
        const uint64_t u = perft(board, depth - 1);
        nodes += u;
        *board = copy;
    }

    return nodes;
}

uint64_t splitPerft(Board *board, const int depth) {
    uint64_t nodes = 0;

    Move moves[256];
    int num_moves = GetMoves(board, moves);
    const Board copy = *board;
    double start = clock();
    for (int i = 0; i < num_moves; i++) {
        MakeMove(board, moves[i]);
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