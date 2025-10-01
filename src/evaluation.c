#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        -1, 1, -3, 3, -2, 2, 1, 1,
        1, 2, 0, 6, -1, 0, 1, 2,
        0, 0, 2, 0, -2, 1, 0, 1,
        -3, -1, 1, 1, -4, -2, 0, 0,
        -1, 15, 13, 7, -4, 11, 1, 9,
        6, 11, 7, -10, 27, 16, 24, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -5, 1, 15, 12, 17, 5, 1, 6,
        6, 0, 0, 3, -5, 6, 3, 6,
        7, 1, 1, 1, 7, 3, 3, 3,
        -2, 11, 3, 1, 0, 6, 15, -7,
        -3, 4, 2, 9, 2, 6, 6, -10,
        11, 4, 4, 24, 5, -8, -5, 20,
        -1, -7, 6, -2, -5, 0, 20, 6,
        -4, -35, -40, -1, 59, 8, -57, -2,
};

int bishop_table[64] = {
        11, 7, 4, 3, 3, 2, 0, 5,
        2, 5, 3, 7, -1, 1, 3, 5,
        6, 5, 11, 3, 3, 0, -2, 3,
        6, 1, 0, 3, 2, 8, 2, 3,
        6, -6, 7, 2, 11, -3, 9, 0,
        6, 9, 2, 11, 10, 11, -2, 9,
        9, 8, 8, 19, 19, -3, 3, -20,
        3, 7, 0, 26, -27, 4, 34, 6,
};

int rook_table[64] = {
        4, 7, 12, 16, 19, 20, 14, 12,
        6, 10, 23, 11, 27, 18, 28, 15,
        7, 18, 29, 19, 10, 6, 11, 24,
        9, 8, 16, 12, 5, 12, 8, 14,
        8, 14, 18, 15, 26, 10, 18, 12,
        14, 16, 16, 13, 11, 2, 27, 13,
        15, 3, 12, 8, 21, 15, 22, 14,
        12, 16, 27, 28, 27, 30, 24, 15,
};

int queen_table[64] = {
        27, 26, 22, 20, 21, 28, 37, 47,
        28, 25, 26, 31, 23, 29, 29, 30,
        27, 25, 29, 40, 36, 28, 32, 26,
        18, 26, 35, 38, 32, 31, 33, 36,
        29, 29, 25, 38, 40, 40, 34, 32,
        27, 34, 31, 32, 31, 39, 39, 36,
        36, 39, 36, 30, 45, 32, 39, 33,
        42, 34, 36, 35, 33, 32, 22, 32,
};

int king_table[64] = {
        -24, -5, -1, 6, 3, 1, 1, 8,
        -14, 8, 8, 6, -1, 3, 1, 1,
        -12, -8, 14, 2, -7, -12, -9, -20,
        -28, -2, 0, 8, 4, -3, -4, -13,
        -12, -4, -37, -3, 5, 5, -9, -16,
        -24, -13, -17, -26, -26, -2, 0, -13,
        -1, 7, -5, 4, 1, -43, -110, -23,
        5, -20, 9, -18, 0, -41, 0, -52,
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
