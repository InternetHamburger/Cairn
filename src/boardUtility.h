#ifndef CAIRN_BOARDUTILITY_H
#define CAIRN_BOARDUTILITY_H

#include "board.h"

bool IsOppositeColor(bool white_to_move, int piece);
bool IsColor(bool white_to_move, int piece);
PieceType GetType(Piece piece);

#endif //CAIRN_BOARDUTILITY_H
