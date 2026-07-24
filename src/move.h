#ifndef CAIRN_MOVE_H
#define CAIRN_MOVE_H

#include "structs.h"

int StartSquare(Move move);
int TargetSquare(Move move);
int GetFlag(Move move);
bool IsPromotion(Move move);
Move MoveConstructor(int start_square, int target_square, MoveFlag flag);

#endif //CAIRN_MOVE_H
