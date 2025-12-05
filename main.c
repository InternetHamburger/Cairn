#include <stdio.h>
#include "src/board.h"
#include "src/datagen.h"
#include "src/uci.h"
#include "src/transposition.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char *args[]) {

    int num_entries = 16 * 1000000 / sizeof(Entry);
    Entry* entries = malloc(num_entries * sizeof(Entry));
    tt.num_entries = num_entries;
    tt.entries = entries;
    ZeroTT();

    int offset = argc > 1 && (strncmp(args[1], "datagen", strlen("datagen")) == 0) ? 1 : 0;

    if (argc > 1 && (strncmp(args[offset], "datagen", strlen("datagen")) == 0)){
        uint64_t seed;
        sscanf(args[2 + offset], "%llu", &seed);
        FILE *file = fopen(args[4 + offset], "ab");

        Thread state = {
                .thread_id = seed,
                .file = file
        };

        GameLoop(&state);
        return 0;
    }

    Board board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Stack stack = {
            .nodes = 0,
            .node_limit = INT64_MAX,
            .print_info = true,
            .depth_limit = 255,
            .soft_node_limit = INT64_MAX,
            .time_limit = INT64_MAX,
            .hash_index = 0
    };

    while (1) {
        char line[20000];
        fflush(stdout);
        fgets(line, 20000, stdin);
        char* input = malloc(strlen(line) + 1);
        for (size_t i = 0; i < strlen(line); i++){
            input[i] = line[i];
        }
        input[strlen(line)] = '\0';
        ReceiveCommand(input, &board, args[0], &stack);

    }
    return 0;
}
