#include <stdio.h>
#include <time.h>
#include "src/board.h"
#include "src/perft.h"
#include "src/uci.h"
#include "src/utility.h"
#include "src/moveGeneration.h"
#include <stdlib.h>
#include <string.h>

int main(void) {


    Board board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 55);

    while (1) {
        char line[200];
        fgets(line, 200, stdin);

        ReciveCommand(line, (int)strlen(line), &board);
    }




    const double start = clock();
    PrintBoard(&board);

    splitPerft(&board, 3);
    const double end = clock();
    printf("Time taken: %lf", (end - start) / CLOCKS_PER_SEC);
    return 0;
}