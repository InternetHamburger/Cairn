#include <stdio.h>
#include "src/board.h"
#include "src/uci.h"
#include <stdlib.h>
#include <string.h>

int main(void) {
    Board board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    while (1) {
        char line[20000];
        fflush(stdout);
        fgets(line, 20000, stdin);
        char* input = malloc(strlen(line) + 1);
        for (int i = 0; i < strlen(line); i++){
            input[i] = line[i];
        }
        input[strlen(line)] = '\0';
        ReceiveCommand(input, &board);
    }
    return 0;
}