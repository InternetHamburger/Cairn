#include <stdio.h>
#include <time.h>
#include "src/board.h"
#include "src/perft.h"


int main(void) {

    Board board = {
        .squares = {12, 10, 11, 13, 14, 11, 10, 12,
                        9, 9, 9, 9, 9, 9, 9, 9,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 1, 0, 0, 0,
                        1, 1, 1, 1, 0, 1, 1, 1,
                        4, 2, 3, 5, 6, 3, 2, 4},
        .white_to_move = true,
        .white_kingside = true,
        .white_queenside = true,
        .black_kingside = true,
        .black_queenside = true,
        .en_passant_square = -1
    };
    const double start = clock();

    printf("%llu\n", perft(&board, 1));
    const double end = clock();
    printf("Time taken: %lf", (end - start) / CLOCKS_PER_SEC);
    return 0;
}
