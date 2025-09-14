#include <stdio.h>
#include "src/board.h"
#include "src/move.h"
#include "src/moveGeneration.h"
#include "src/boardUtility.h"

int main(void) {

    Board board = {
            .squares = {4, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0},
            .white_to_move = true
    };

    int num_moves = 0;
    Move* moves = GetMoves(&board, &num_moves);
    printf("%d\n", num_moves);
    for (int i = 0; i < num_moves; i++){
        printf("%d %d\n", StartSquare(moves[i]), TargetSquare(moves[i]));
    }

    return 0;
}
