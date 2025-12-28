#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        82, 85, 84, 60, 52, 74, 66, 72,
        30, 21, 21, 16, 18, 8, 15, 4,
        -11, -9, -18, -15, -15, -21, -17, -26,
        -20, -19, -21, -20, -21, -24, -21, -33,
        -22, -17, -24, -25, -22, -24, -14, -32,
        -24, -17, -27, -39, -32, -14, -11, -34,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -88, 2, 0, 1, 13, -3, -18, -57,
        -16, 1, 12, 19, 13, 21, 3, 0,
        -5, 17, 23, 33, 27, 28, 25, 2,
        -1, 11, 24, 26, 25, 34, 19, 15,
        -7, 10, 18, 14, 18, 15, 16, 0,
        -17, -1, 0, 12, 11, 4, 6, -15,
        -26, -14, -6, -2, -1, 2, -8, -19,
        -41, -28, -22, -20, -15, -17, -26, -35,
};

int bishop_table[64] = {
        6, 0, 0, 3, -4, -8, 0, -5,
        -7, 2, 7, 4, 6, 2, -3, -12,
        -3, 7, 8, 13, 12, 17, 15, 7,
        -5, 6, 11, 19, 17, 15, 9, 1,
        -4, 4, 10, 14, 14, 4, 5, -2,
        -4, 1, 1, 6, 1, 5, 0, -1,
        -10, -5, 0, -5, -1, 0, 1, -21,
        -20, -11, -19, -12, -13, -19, -29, -19,
};

int rook_table[64] = {
        4, 12, 19, 17, 17, 18, 16, 11,
        20, 21, 26, 25, 24, 23, 16, 21,
        14, 14, 19, 19, 19, 22, 15, 10,
        0, 6, 9, 11, 5, 10, 5, 1,
        -13, -8, -3, 0, -3, -4, -8, -13,
        -23, -15, -13, -14, -13, -19, -14, -20,
        -28, -20, -16, -17, -18, -18, -20, -37,
        -20, -16, -12, -10, -9, -13, -25, -27,
};

int queen_table[64] = {
        -1, 10, 21, 27, 31, 33, 40, 27,
        -11, -2, 15, 17, 29, 41, 39, 36,
        -12, 0, 11, 20, 30, 46, 38, 24,
        -16, -8, 0, 5, 23, 19, 12, 6,
        -17, -12, -5, 0, 1, 0, -1, -7,
        -22, -14, -11, -10, -8, -7, -9, -15,
        -28, -20, -12, -15, -14, -11, -32, -31,
        -19, -28, -25, -14, -27, -44, -53, -38,
};

int king_table[64] = {
        -55, 3, 0, 4, 25, 26, 24, -99,
        -13, 14, 8, 11, 13, 26, 29, -7,
        6, 21, 19, 13, 24, 23, 37, 9,
        -3, 18, 14, 13, 18, 21, 22, 1,
        -13, 3, 6, 12, 12, 10, 5, -6,
        -18, -4, -3, -2, 0, 0, -1, -14,
        -10, -9, -12, -18, -13, -7, -3, -11,
        -38, -22, -14, -35, -15, -26, -1, -16,
};

const int piece_values[] = {0, 100, 234, 252, 385, 712, 0};

int piece_square_values[BlackKing + 1][64];

__attribute__((constructor))
static void init_table(void) {
    for (int i = 0; i < 64; i++)
    {
        piece_square_values[WhitePawn][i] = pawn_table[i] + piece_values[Pawn];
        piece_square_values[WhiteKnight][i] = knight_table[i] + piece_values[Knight];
        piece_square_values[WhiteBishop][i] = bishop_table[i] + piece_values[Bishop];
        piece_square_values[WhiteRook][i] = rook_table[i] + piece_values[Rook];
        piece_square_values[WhiteQueen][i] = queen_table[i] + piece_values[Queen];
        piece_square_values[WhiteKing][i] = king_table[i] + piece_values[King];

        piece_square_values[BlackPawn][i] = -pawn_table[FlipSquare(i)] - piece_values[Pawn];
        piece_square_values[BlackKnight][i] = -knight_table[FlipSquare(i)] - piece_values[Knight];
        piece_square_values[BlackBishop][i] = -bishop_table[FlipSquare(i)] - piece_values[Bishop];
        piece_square_values[BlackRook][i] = -rook_table[FlipSquare(i)] - piece_values[Rook];
        piece_square_values[BlackQueen][i] = -queen_table[FlipSquare(i)] - piece_values[Queen];
        piece_square_values[BlackKing][i] = -king_table[FlipSquare(i)] - piece_values[King];
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
