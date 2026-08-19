#include <stdio.h>
#include "src/board.h"
#include "src/uci.h"
#include "src/transposition.h"
#include "src/nnue.h"
#include <stdlib.h>
#include <string.h>
#ifdef __unix__
    #include <sys/resource.h>
#endif

int main(int argc, char *args[]) {
    #ifdef __unix__
        struct rlimit rl;
        getrlimit(RLIMIT_STACK, &rl);
        rl.rlim_cur = 18 * 1024 * 1024; // 18 MB
        if (setrlimit(RLIMIT_STACK, &rl) != 0){
            perror("Setting new stack limit failed");
            return 1;
        }
    #endif
    
    load_incbin();
    const Board board = BoardConstructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Thread thread = {
            .nodes = 0,
            .node_limit = INT64_MAX,
            .print_info = true,
            .depth_limit = 255,
            .soft_node_limit = INT64_MAX,
            .time_limit = INT64_MAX,
            .soft_time_limit = INT64_MAX,
            .board = board,
    };

    int num_entries = 16 * 1000000 / sizeof(Entry);
    Entry* entries = malloc(num_entries * sizeof(Entry));
    thread.tt.num_entries = num_entries;
    thread.tt.entries = entries;
    ZeroTT(&thread.tt);

    if (argc > 1 && strncmp(args[1], "bench", 5) == 0){
        Bench(&thread);
        return 0;
    }
    if (argc > 1 && strncmp(args[1], "perftsuite", 10) == 0){
        PerftSuite();
        return 0;
    }


    while (1) {
        char line[20000];
        fflush(stdout);
        fgets(line, 20000, stdin);
        char* input = malloc(strlen(line) + 1);
        for (size_t i = 0; i < strlen(line); i++){
            input[i] = line[i];
        }
        input[strlen(line)] = '\0';
        ReceiveCommand(input, &thread);

    }
    return 0;
}
