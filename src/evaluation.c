#include "evaluation.h"

#include <stdio.h>

#include "utility.h"

#include "board.h"

const int PIECE_VALUES[] = {0, 100, 300, 300, 500, 900, 0};

int eval(Board *board) {

    int eval = 0;
    for (int i = 0; i < 64; i++) {
        if (IsColor(board->white_to_move, board->squares[i])) {
            eval += PIECE_VALUES[GetType(board->squares[i])];
        } else if (IsOppositeColor(board->white_to_move, board->squares[i])){
            eval -= PIECE_VALUES[GetType(board->squares[i])];
        }
    }
    return eval;
}
