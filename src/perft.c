#include "perft.h"

#include <stdio.h>

#include "board.h"
#include "move.h"
#include "moveGeneration.h"

unsigned long long perft(Board *board, const int depth) {
    if (depth == 0) return 1;

    unsigned long long nodes = 0;

    int num_moves = 0;
    const Move* moves = GetMoves(board, &num_moves);
    const Board copy = *board;
    for (int i = 0; i < num_moves; i++) {
        MakeMove(board, moves[i]);
        if (IsAttackedBySideToMove(board, board->white_to_move ? board->black_king_square : board->white_king_square)) {
            //*board = copy;
            //continue;
        }
        const unsigned long long u = perft(board, depth - 1);
        nodes += u;
        *board = copy;
    }

    return nodes;
}