#ifndef CAIRN_UCI_H
#define CAIRN_UCI_H

#include "search.h"

void Bench(Thread* thread);
void PerftSuite();
void RunDatagen(int argc, char* args[]);
void ReceiveCommand(char* line, Thread *thread);


#endif //CAIRN_UCI_H