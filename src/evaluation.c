#include "evaluation.h"
#include "utility.h"
#include "board.h"

const int PIECE_VALUES[] = {0, 100, 300, 300, 500, 900, 0, 0, 0, -100, -300, -300, -500, -900, 0};

int eval(Board *board) {
    uint64_t occupied = GetOccupied(board);

    int eval = 0;
    while (occupied){
        const int square = poplsb(&occupied);
        eval += PIECE_VALUES[board->squares[square]];
    }
    return eval;
}
