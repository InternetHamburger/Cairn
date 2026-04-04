#ifndef CAIRN_UCI_H
#define CAIRN_UCI_H
#include "board.h"
#include "search.h"

void ReceiveCommand(char* line, char* this_path, Thread *thread);


#endif //CAIRN_UCI_H