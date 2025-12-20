#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        94, 90, 78, 70, 59, 52, 29, 50,
        44, 27, 25, 22, 25, 19, 16, 20,
        -5, -6, -15, -13, -14, -18, -22, -24,
        -17, -16, -20, -17, -22, -25, -24, -34,
        -18, -16, -24, -26, -23, -30, -15, -31,
        -24, -18, -31, -43, -39, -14, -10, -36,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -80, -21, 2, -3, -14, -44, 35, -63,
        -35, 13, 7, 15, 8, 8, -26, 8,
        5, 16, 20, 34, 25, 41, 29, 3,
        0, 14, 30, 31, 31, 31, 19, 21,
        -1, 1, 21, 15, 23, 20, 15, -3,
        -14, -1, 3, 20, 6, 11, 14, -18,
        -50, -10, 0, 2, 2, 6, -5, -23,
        -77, -18, -30, -8, -9, -15, -29, -11,
};

int bishop_table[64] = {
        -9, 21, -4, 1, 8, -13, -25, 14,
        -9, 3, 10, 1, 0, 4, 0, 0,
        7, 6, 14, 14, 0, 15, 35, 11,
        -1, 17, 14, 21, 17, 9, 7, -1,
        0, 6, 16, 15, 15, 8, 5, -4,
        -12, 4, 0, 6, 5, 5, -5, 1,
        -25, -5, -6, -7, -2, -6, 7, -37,
        -18, -20, -17, -15, -21, -18, -37, -30,
};

int rook_table[64] = {
        2, 17, 17, 16, 21, 19, 12, 18,
        17, 20, 25, 23, 23, 12, 20, 17,
        15, 13, 18, 21, 15, 17, -1, 16,
        0, 7, 0, 8, 0, 12, -1, 0,
        -8, 1, -6, -1, -1, -5, 0, -12,
        -23, -17, -5, -15, -11, -14, -7, -21,
        -23, -19, -11, -12, -16, -17, -12, -37,
        -16, -11, -8, -8, -3, -12, -22, -29,
};

int queen_table[64] = {
        -15, 13, -4, 27, 24, 28, 13, 31,
        -3, -13, 25, 11, 35, 52, 49, 38,
        -15, -4, 5, 11, 28, 43, 37, 26,
        -11, -8, 10, 11, 23, 12, 13, 17,
        -13, -3, -9, 4, 0, 1, 6, -8,
        -20, -12, -8, -3, -7, -7, -4, -14,
        -23, -18, -7, -12, -11, -3, -13, -20,
        -14, -37, -22, -10, -17, -25, -59, -83,
};

int king_table[64] = {
        -45, -69, -34, 16, 30, 31, 38, -112,
        20, 8, -9, 0, 24, 41, 35, 20,
        0, 11, 7, 16, 13, 44, 62, 26,
        -9, 10, 9, 13, 24, 27, 23, -5,
        -25, 3, 0, 16, 16, 16, 4, -8,
        -18, -19, -3, -2, -3, 1, 3, -9,
        -9, -13, -6, -27, -19, -2, -1, -5,
        -25, -32, -20, -40, -15, -19, 1, -5,
};

const int piece_values[] = {0, 100, 219, 246, 369, 662, 0};

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
