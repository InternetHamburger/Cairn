#ifndef CAIRN_BOARD_H
#define CAIRN_BOARD_H

#include <stdint.h>
#include "move.h"

#define MAX_NUM_PLY 17697

enum{
    None = 0b0000,
    WhitePawn = 0b0001,
    WhiteKnight = 0b0010,
    WhiteBishop = 0b0011,
    WhiteRook = 0b0100,
    WhiteQueen = 0b0101,
    WhiteKing = 0b0110,
    BlackPawn = 0b1001,
    BlackKnight = 0b1010,
    BlackBishop = 0b1011,
    BlackRook = 0b1100,
    BlackQueen = 0b1101,
    BlackKing = 0b1110,
};

typedef uint8_t Piece;

enum{
    Pawn = 0b0001,
    Knight = 0b0010,
    Bishop = 0b0011,
    Rook = 0b0100,
    Queen = 0b0101,
    King = 0b0110,
};

typedef uint8_t PieceType;

typedef struct{
    Piece squares[64];
    bool white_to_move;
    bool white_kingside;
    bool white_queenside;
    bool black_kingside;
    bool black_queenside;
    int en_passant_square;
    int white_king_square;
    int black_king_square;
    uint64_t bitboards[BlackKing + 1];
    uint64_t zobrist_hash;
    int fifty_move_counter;
} Board;

void MakeMove(Board *board, Move move);
void PrintBoard(const Board* board);
bool InCheck(const Board *board);
bool IsAttackedBySideToMove(const Board *board, bool white_to_move, int square);
Board BoardConstructor(const char* fen);
bool IsRepetition(const uint64_t hashes[MAX_NUM_PLY], int idx);
PieceType PromotionType(Move move);
uint64_t GetOccupied(const Board *board);
uint64_t GetWhiteBitboard(const Board *board);
uint64_t GetBlackBitboard(const Board *board);
int staticExchangeEvaluation(Board *board, Move move, int threshold);


#endif //CAIRN_BOARD_H
