#include <time.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>

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
        printf("%s: %"PRIu64"\n", MoveToString(moves[i]), u);
        nodes += u;
        *board = copy;
    }
    double elapsed = clock() - start;
    printf("\nNodes searched: %"PRIu64"\n", nodes);
    printf("Knps: %lf\n", round((double)nodes / elapsed));
    return nodes;
}