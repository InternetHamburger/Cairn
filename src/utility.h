#ifndef CAIRN_UTILITY_H
#define CAIRN_UTILITY_H

#include "board.h"

bool IsOppositeColor(bool white_to_move, int piece);
bool IsColor(bool white_to_move, int piece);
int FlipSquare(int square);
char PieceToChar(Piece piece);
Piece CharToPiece(char piece);
char* SquareToString(int square);
int StringToSquare(char* square);
char* MoveToString(Move move);
bool IsLegalCastle(const Board *board, Move move);
PieceType GetType(Piece piece);
int poplsb(unsigned long long *bb);
Move StringToMove(char* move, Board *board);

#endif //CAIRN_UTILITY_H
