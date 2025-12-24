#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        70, 84, 83, 62, 54, 72, 60, 69,
        37, 22, 19, 17, 17, 18, 19, 10,
        -12, -7, -21, -15, -13, -20, -18, -27,
        -21, -22, -24, -20, -22, -24, -23, -35,
        -23, -20, -25, -29, -23, -24, -14, -30,
        -25, -18, -28, -42, -34, -13, -12, -34,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -71, -15, 14, 6, 4, 19, -39, -75,
        -24, 1, 11, 25, 17, 17, -8, 8,
        -7, 23, 20, 29, 37, 16, 25, 10,
        -1, 10, 22, 25, 26, 30, 23, 15,
        -2, 4, 17, 13, 16, 14, 21, 1,
        -17, -2, -1, 15, 8, 7, 8, -16,
        -30, -14, -8, -2, -2, 2, -6, -20,
        -28, -25, -22, -20, -13, -25, -22, -21,
};

int bishop_table[64] = {
        15, 4, 2, 3, -15, -8, -1, 5,
        -6, 7, 5, 7, 0, -5, 7, -22,
        1, 2, 11, 11, 6, 23, 10, 8,
        -3, 6, 8, 19, 18, 10, 7, 5,
        -5, 3, 11, 16, 10, 4, 5, 0,
        -6, -4, 0, 6, 3, 4, 0, 0,
        -20, -4, 0, -6, -3, 2, 2, -11,
        -24, -17, -18, -12, -12, -21, -28, -19,
};

int rook_table[64] = {
        7, 16, 18, 17, 17, 17, 22, 16,
        20, 23, 28, 24, 28, 23, 22, 19,
        17, 18, 23, 20, 19, 18, 16, 14,
        3, 9, 10, 12, 7, 7, 3, 2,
        -10, -8, -5, -5, -4, -9, -9, -15,
        -21, -16, -14, -15, -11, -20, -17, -20,
        -27, -21, -16, -17, -19, -18, -26, -48,
        -18, -14, -10, -9, -8, -12, -27, -28,
};

int queen_table[64] = {
        3, 14, 20, 37, 31, 25, 37, 38,
        -15, -2, 20, 22, 23, 50, 38, 42,
        -10, -3, 8, 13, 31, 49, 40, 20,
        -15, -7, 4, 8, 20, 16, 17, 8,
        -18, -13, -9, 0, 3, 2, 0, -9,
        -27, -17, -9, -11, -10, -9, -9, -12,
        -31, -19, -13, -18, -15, -11, -24, -31,
        -20, -28, -24, -15, -27, -36, -54, -29,
};

int king_table[64] = {
        0, -3, 10, -4, -8, 6, -25, -69,
        -10, 14, 12, -2, 6, 1, 17, -22,
        12, 18, 20, 14, 9, 27, 14, 15,
        -4, 18, 16, 19, 18, 22, 25, 0,
        -9, 0, 15, 12, 16, 15, 9, -5,
        -17, 2, 0, 3, 1, 2, 1, -9,
        -9, -8, -12, -14, -9, -3, -1, -6,
        -40, -25, -12, -28, -10, -17, 1, -8,
};

const int piece_values[] = {0, 100, 236, 256, 386, 708, 0};

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
