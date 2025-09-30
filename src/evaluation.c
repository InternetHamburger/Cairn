#include "evaluation.h"
#include "utility.h"
#include "board.h"

const int PIECE_VALUES[] = {0, 100, 300, 300, 500, 900, 0, 0, 0, -100, -300, -300, -500, -900, 0};
int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        -6, 2, -3, -16, -5, -8, -3, -4,
        -12, 6, -10, -3, -12, -19, 6, -14,
        -11, -4, 1, -11, -20, 1, 5, -9,
        4, 12, 5, 20, 39, 8, 10, 18,
        7, -11, 12, 17, 28, 33, 45, 12,
        98, 66, 20, 46, 31, 40, 110, -5,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -166, 10, 29, 10, 26, 47, 26, 68,
        37, 35, 34, 24, 27, -15, 119, 18,
        3, 52, 18, 37, -14, -35, 0, 9,
        19, 75, 30, 22, 12, -9, 90, 37,
        63, 15, 24, 9, 10, 53, 18, 15,
        -7, 32, 32, 5, 35, 10, 3, 27,
        27, 69, 45, 39, 68, 56, 30, 62,
        47, 65, 38, 54, 53, 11, 61, 25,
};

int bishop_table[64] = {
        42, 139, -6, -5, 14, 7, -49, -42,
        198, 13, 9, -23, 2, -6, 21, 51,
        17, -67, -13, 7, -12, 19, 68, -25,
        0, 40, 40, 17, 25, 31, 27, 9,
        69, 30, 30, 27, 38, 19, 4, 15,
        41, 22, 39, 45, 29, 24, 14, 28,
        36, 56, 28, 3, 27, 33, 23, 23,
        51, 39, 92, 18, 2, 17, 66, 47,
};

int rook_table[64] = {
        -17, -15, -37, 24, -4, -25, 27, 0,
        -5, -13, 13, 3, 40, -28, 46, 9,
        -7, -7, -12, 5, -7, 5, -100, -19,
        71, 87, 22, -25, 4, 79, 24, -25,
        55, 21, -7, 35, -3, -12, -2, 60,
        -3, -46, -25, 11, 18, 11, 31, 71,
        33, 29, 108, 69, 36, 58, 24, 53,
        -1, 49, -20, 19, -41, 40, 32, -64,
};

int queen_table[64] = {
        60, 47, 71, 57, 28, 93, 93, -22,
        19, 65, 91, 23, 65, 32, 113, -7,
        95, 66, 94, 69, 75, 52, 80, -30,
        65, 118, 81, 87, 127, 104, 75, 87,
        66, 78, 91, 81, 87, 86, 105, 84,
        71, 94, 94, 77, 55, 66, 96, 94,
        96, 123, 75, 38, 43, 107, 94, 55,
        109, 109, 92, 21, 71, 58, 60, 83,
};

int king_table[64] = {
        4, -9, -2, 4, 35, 40, -21, -53,
        12, 12, 20, -25, -37, 29, 7, -5,
        -3, -17, 4, 2, 25, -22, -40, -23,
        31, -9, -10, -20, -8, -1, -43, -29,
        -68, 8, -72, 6, -6, -37, -55, -54,
        -35, 25, 65, 109, 7, -12, 44, -8,
        -83, -30, 198, 41, 28, 0, 1, -15,
        -42, 0, 23, 0, 0, 198, 47, -39,
};

int piece_square_values[BlackKing + 1][64];

__attribute__((constructor))  // runs before main()
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

int eval(Board *board) {
    uint64_t occupied = GetOccupied(board);

    int eval = 0;
    while (occupied){
        const int square = poplsb(&occupied);
        eval += piece_square_values[board->squares[square]][square];
    }
    return eval;
}
