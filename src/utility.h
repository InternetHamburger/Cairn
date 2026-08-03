#ifndef CAIRN_UTILITY_H
#define CAIRN_UTILITY_H

#include "board.h"

bool IsOppositeColor(bool white_to_move, int piece);
bool IsMinor(Piece piece);
bool GetColor(Piece piece);
int GetFile(int sq);
int GetRank(int sq);
int FlipFile(int sq);
bool one_bit_set(uint64_t num);
bool IsColor(bool white_to_move, int piece);
int FlipRank(int square);
char PieceToChar(Piece piece);
Piece CharToPiece(char piece);
char* SquareToString(int square);
int StringToSquare(char* square);
char* MoveToString(Move move);
PieceType GetType(Piece piece);
int getlsb(uint64_t bb);
int poplsb(uint64_t *bb);
bool is_square_attacked(const Board* board, int sq);
Move StringToMove(char* move, Board *board);
char* BoardToFen(const Board *board);
void PrintBitBoard(uint64_t bitboard);
bool IsDiagonalSlider(Piece piece);
bool IsOrthogonalSlider(Piece piece);

#endif //CAIRN_UTILITY_H
