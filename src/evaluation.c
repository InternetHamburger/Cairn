#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        95, 96, 81, 62, 62, 52, 50, 67,
        40, 32, 35, 33, 16, 0, 13, 27,
        -1, -2, -10, -13, -19, -21, -24, -26,
        -14, -21, -23, -17, -21, -32, -26, -36,
        -16, -16, -20, -31, -27, -42, -18, -34,
        -27, -15, -35, -45, -49, -18, -11, -37,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -70, -11, -12, 10, -4, -17, 15, -10,
        -12, 5, 7, 22, 17, 19, 9, 19,
        4, 19, 15, 33, 19, 17, 20, 15,
        -10, 4, 16, 25, 33, 30, 15, 21,
        -3, 5, 21, 15, 15, 11, 23, -3,
        -22, -10, -2, 12, 6, 10, 10, -23,
        -65, -18, -15, -4, -5, -3, -20, -20,
        -31, -31, -30, -13, -21, -22, -35, 4,
};

int bishop_table[64] = {
        -7, -8, -8, 1, -2, 4, -25, -30,
        -13, 11, 17, -4, 7, 9, -2, -7,
        -5, 21, 14, 19, 4, 13, 29, 11,
        0, 19, 20, 25, 15, 17, 11, -5,
        -12, 8, 15, 24, 16, 5, -1, -9,
        -5, 5, 2, 11, 3, 3, -8, -4,
        -4, -11, -7, -7, -5, 2, 9, -30,
        -34, -28, -23, -14, -10, -22, -22, -28,
};

int rook_table[64] = {
        1, 17, 20, 18, 17, 14, 4, 5,
        21, 24, 32, 23, 30, 15, 5, 14,
        17, 14, 21, 16, 16, 8, 6, 10,
        0, 10, 11, 9, 12, 14, 1, 3,
        -10, -6, -3, 1, -4, -7, -6, -7,
        -22, -15, -6, -10, -4, -18, -7, -32,
        -25, -18, -12, -13, -19, -23, -23, -44,
        -17, -13, -6, -5, -1, -12, -31, -33,
};

int queen_table[64] = {
        3, 9, 15, 6, 36, 34, 59, 13,
        -1, -12, 12, 10, 30, 34, 67, 22,
        3, -12, 7, 21, 38, 51, 36, 22,
        -18, -6, 2, 4, 15, 9, 14, 11,
        -10, -10, -5, 8, -2, -4, 10, -9,
        -13, -14, -7, -7, -12, -9, -5, -16,
        -12, -21, -8, -14, -10, -4, -44, -44,
        -24, -30, -23, -11, -24, -40, -69, -47,
};

int king_table[64] = {
        -34, -56, -3, -19, -2, 4, -15, -128,
        -5, 26, 46, 1, 12, 15, 17, 0,
        33, 38, 31, 24, 18, 27, 18, 6,
        19, 32, 24, 23, 22, 24, 21, 2,
        -15, 13, 11, 15, 17, 18, 3, -5,
        -16, -2, -1, -3, 0, 4, 4, -1,
        -48, -6, -9, -27, -17, -2, 0, -6,
        -21, -31, -28, -45, -17, -20, 5, -5,
};

const int piece_values[] = {0, 102, 230, 250, 369, 657, 0};

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
