#ifndef CAIRN_SEARCH_H
#define CAIRN_SEARCH_H

#include "transposition.h"

int correct_eval(Thread* thread, int eval, int ply);
SearchResult search(Thread *thread);

#endif //CAIRN_SEARCH_H