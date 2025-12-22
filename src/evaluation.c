#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        79, 88, 67, 61, 72, 75, 50, 64,
        41, 15, 19, 13, 15, 23, 26, 9,
        -11, -5, -21, -14, -11, -21, -17, -25,
        -21, -20, -24, -20, -23, -22, -21, -31,
        -23, -22, -26, -29, -22, -23, -10, -27,
        -25, -19, -28, -43, -32, -13, -9, -31,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -70, -3, 8, 0, -5, 9, -64, -53,
        -28, 5, 6, 26, 17, 25, -19, 15,
        -8, 24, 12, 29, 45, 20, 25, 1,
        -2, 14, 23, 28, 26, 32, 28, 12,
        -4, 11, 18, 18, 18, 17, 22, 6,
        -15, 1, 0, 15, 7, 10, 8, -17,
        -42, -13, -8, 2, 0, 1, -5, -13,
        -39, -24, -25, -22, -9, -19, -18, -45,
};

int bishop_table[64] = {
        10, 8, 3, 8, -9, -10, 3, 1,
        -7, 1, 11, 3, 3, -3, 4, -26,
        0, 7, 6, 13, 11, 23, 9, 10,
        2, 3, 9, 22, 18, 9, 1, 6,
        -3, 9, 9, 15, 10, 5, 4, 0,
        -10, -2, -2, 5, 4, 3, 2, -1,
        -34, -5, 2, -5, -4, 9, 1, -10,
        -29, -23, -18, -14, -7, -20, -52, -25,
};

int rook_table[64] = {
        4, 19, 19, 21, 19, 19, 22, 15,
        18, 20, 26, 25, 26, 22, 22, 14,
        20, 17, 27, 23, 20, 17, 21, 17,
        0, 6, 8, 15, 5, 8, 2, -3,
        -10, -8, -4, -2, -5, -6, -11, -14,
        -21, -17, -13, -18, -11, -21, -15, -19,
        -26, -22, -15, -17, -20, -19, -24, -47,
        -19, -14, -9, -11, -7, -12, -28, -29,
};

int queen_table[64] = {
        5, 18, 15, 47, 45, 30, 42, 35,
        -15, 3, 21, 27, 27, 45, 47, 37,
        -14, -7, 9, 11, 24, 52, 31, 11,
        -14, -13, 0, 7, 25, 11, 13, 7,
        -17, -12, -7, 0, 4, 0, 0, -12,
        -28, -21, -14, -14, -12, -8, -10, -19,
        -30, -15, -14, -19, -19, -12, -24, -26,
        -25, -26, -24, -17, -26, -40, -59, -45,
};

int king_table[64] = {
        -12, -18, 21, -8, -6, 12, -23, -87,
        -18, 18, 6, 0, 3, 0, 39, -30,
        0, 25, 27, 13, 0, 29, 15, 20,
        -15, 22, 17, 22, 17, 22, 32, 0,
        -10, 0, 20, 12, 20, 17, 8, -5,
        -12, 11, 0, 3, 3, 5, 1, -6,
        -11, -6, -12, -14, -8, -3, 0, -6,
        -58, -29, -15, -28, -9, -17, 2, -5,
};

const int piece_values[] = {0, 101, 236, 259, 390, 706, 0};

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
