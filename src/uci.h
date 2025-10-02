#ifndef CAIRN_UCI_H
#define CAIRN_UCI_H
#include "board.h"
#include "search.h"

void ReceiveCommand(char* line, Board *board, char* this_path, Stack *stack);

typedef struct {
    void (*function)(void *arg);
    void *arg;
} task_t;

#endif //CAIRN_UCI_H