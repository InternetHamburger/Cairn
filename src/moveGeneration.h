#ifndef CAIRN_MOVEGENERATION_H
#define CAIRN_MOVEGENERATION_H

int GetMoves(const Board *board, Move* moves);
int GetCaptures(const Board *board, Move* moves);
int GetQuiets(const Board *board, Move* moves);

#endif //CAIRN_MOVEGENERATION_H
