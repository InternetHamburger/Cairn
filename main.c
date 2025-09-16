#include <stdio.h>
#include <time.h>
#include "src/board.h"
#include "src/perft.h"
#include "src/utility.h"
#include "src/moveGeneration.h"
#include <stdlib.h>

int main(void) {
    char line[200];
    scanf(" %199s ", line);
    printf("hmm");


    int empty_encounters = 0;
    int fen_length = 0;
    for (int i = 0; i < 0; i++){
        if (line[i] == ' ') empty_encounters++;
        if (empty_encounters == 5) {
            fen_length = i + 1;
            break;
        }
    }

    Board board = BoardConstructor(line, fen_length);

    const double start = clock();
    PrintBoard(&board);

    splitPerft(&board, 6);
    const double end = clock();
    printf("Time taken: %lf", (end - start) / CLOCKS_PER_SEC);
    free(line);
    return 0;
}
