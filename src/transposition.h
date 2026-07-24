#ifndef CAIRN_TRANSPOSITION_H
#define CAIRN_TRANSPOSITION_H

#include "structs.h"

#define EXACT 0b01000000
#define UPPER 0b10000000
#define LOWER 0b11000000

int GetDepth(Entry entry);
int GetEntryType(Entry entry);
void ZeroTT(TT* tt);
bool IsNull(Entry entry);

#endif //CAIRN_TRANSPOSITION_H
