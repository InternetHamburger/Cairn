#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        -1, 1, -1, -2, -1, 2, 0, 1,
        0, 2, 0, -1, 0, 5, -2, 2,
        -1, 1, -3, 1, 1, 6, 0, 0,
        -2, 2, 1, 4, 1, 1, 8, 2,
        -3, 4, 0, 7, 12, 9, -2, 10,
        1, -15, 4, 7, 4, 5, -2, -15,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        5, 0, 17, 12, 2, 5, -3, 2,
        6, 10, 6, -1, 6, 6, -2, 3,
        4, 4, 2, -2, 13, 2, 10, 3,
        5, 6, 9, 5, 8, 10, 13, -1,
        18, 6, 15, 2, 14, 6, 12, 11,
        0, 5, 11, 7, 11, 14, 12, 13,
        12, 7, -1, 16, 16, 5, 4, 4,
        6, -13, -2, -17, -8, 8, 8, 5,
};

int bishop_table[64] = {
        15, -2, 7, 3, 4, 4, -2, 5,
        4, 10, 4, 3, 5, 11, 5, 6,
        14, 10, 9, 4, 10, 5, 9, 11,
        -2, 10, 12, 9, 9, 4, 8, 11,
        -4, 4, 6, 9, 14, 12, 11, 4,
        7, 18, 10, 23, 10, 19, 7, 9,
        11, 18, 9, 14, 19, 15, 10, -1,
        8, 6, 15, 6, 18, 9, 16, 5,
};

int rook_table[64] = {
        9, 12, 10, 10, 13, 12, 12, 12,
        11, 11, 14, 13, 8, 11, 10, 12,
        11, 19, 9, 15, 16, 11, 15, 13,
        16, 24, 16, 24, 12, 19, 20, 17,
        17, 23, 15, 19, 16, 28, 18, 14,
        18, 23, 16, 26, 27, 29, 22, 21,
        20, 22, 24, 26, 19, 37, 24, 20,
        28, 27, 12, 33, 20, 33, 16, 1,
};

int queen_table[64] = {
        22, 26, 24, 24, 24, 28, 17, 55,
        26, 24, 28, 22, 27, 28, 25, 52,
        21, 35, 26, 32, 26, 30, 36, 33,
        34, 37, 39, 36, 33, 33, 34, 32,
        31, 35, 41, 37, 38, 36, 40, 33,
        34, 36, 42, 44, 37, 45, 39, 39,
        38, 39, 31, 43, 48, 37, 39, 36,
        47, 44, 36, 52, 42, 42, 37, 32,
};

int king_table[64] = {
        3, -4, 5, 0, 2, 0, 6, -6,
        -1, 4, 0, 1, 6, -1, -9, -1,
        -3, -2, -1, -9, -10, -9, -7, -15,
        -15, 2, -11, -14, -7, -2, -14, -12,
        -33, -13, 0, 6, -2, -10, -27, -39,
        -22, -9, 5, -5, 6, 4, -1, -43,
        -30, 3, 5, 3, 0, 20, 1, 3,
        -34, -3, -3, 8, 22, 11, 27, 64,
};

int piece_square_values[BlackKing + 1][64];

__attribute__((constructor))
static void init_table(void) {
    for (int i = 0; i < 64; i++)
    {
        piece_square_values[WhitePawn][i] = 100 + pawn_table[i];
        piece_square_values[WhiteKnight][i] = 300 + knight_table[i];
        piece_square_values[WhiteBishop][i] = 300 + bishop_table[i];
        piece_square_values[WhiteRook][i] = 500 + rook_table[i];
        piece_square_values[WhiteQueen][i] = 900 + queen_table[i];
        piece_square_values[WhiteKing][i] = 0 + king_table[i];

        piece_square_values[BlackPawn][i] = -100 - pawn_table[FlipSquare(i)];
        piece_square_values[BlackKnight][i] = -300 - knight_table[FlipSquare(i)];
        piece_square_values[BlackBishop][i] = -300 - bishop_table[FlipSquare(i)];
        piece_square_values[BlackRook][i] = -500 - rook_table[FlipSquare(i)];
        piece_square_values[BlackQueen][i] = -900 - queen_table[FlipSquare(i)];
        piece_square_values[BlackKing][i] = 0 - king_table[FlipSquare(i)];
    }
}

int eval(const Board *board) {
    uint64_t occupied = GetOccupied(board);

    int eval = 0;
    while (occupied){
        const int square = poplsb(&occupied);
        eval += piece_square_values[board->squares[square]][square];
    }
    return eval * (board->white_to_move ? 1 : -1);
}
