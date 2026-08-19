#ifndef CAIRN_BOARD_H
#define CAIRN_BOARD_H

#include <stdint.h>
#include "move.h"

uint64_t GetCheckers(const Board* board, int sq);
uint64_t get_threat_bitboard(const Board *board);
void MakeMove(Board *board, Move move);
void MakeNullMove(Board *board);
void PrintBoard(const Board* board);
bool IsPseudoLegal(const Board* board, Move move);
bool IsLegal(const Board* board, Move move);
bool InCheck(const Board *board);
Board BoardConstructor(const char* fen);
bool IsRepetition(const uint64_t hashes[MAX_NUM_PLY], int idx);
bool IsDraw(const uint64_t hashes[MAX_NUM_PLY], const Board* board);
PieceType PromotionType(Move move);
uint64_t GetOccupied(const Board *board);
bool HasNonPawnKing(const Board *board);
uint64_t AttackersToSquare(const Board *board, int square, uint64_t occupied);
int staticExchangeEvaluation(Board *board, Move move, int threshold);


#endif //CAIRN_BOARD_H
