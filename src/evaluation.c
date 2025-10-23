#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        93, 103, 114, 94, 56, 82, 62, 75,
        47, 62, 60, 44, 19, -3, 14, 36,
        2, -4, -13, -18, -20, -26, -24, -27,
        -13, -21, -28, -13, -28, -41, -42, -43,
        -22, -15, -30, -38, -42, -63, -21, -40,
        -33, -17, -51, -63, -64, -28, -15, -47,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -57, -74, -15, -17, -12, -39, 9, 2,
        -36, 13, 16, 27, 3, 9, -29, -33,
        -17, 35, 32, 33, -35, 18, 32, -10,
        -6, 4, 21, 40, 48, 43, 24, 22,
        -17, 3, 35, 35, 30, 27, 19, 3,
        -14, -10, 11, 20, 25, 29, 21, -10,
        -35, 4, 21, 9, 0, 0, -26, -14,
        -32, -23, -57, -17, -13, -31, -15, -55,
};

int bishop_table[64] = {
        5, -8, -10, 3, -20, -8, -31, -9,
        -33, 0, 10, 8, 4, 5, -4, -21,
        -1, 18, 21, 23, 8, 17, 16, 18,
        0, 17, 21, 22, 29, 20, 14, 2,
        -7, 22, 21, 29, 13, 10, -1, -17,
        0, 16, 10, 14, -1, 13, -12, 16,
        -7, -4, -1, -6, -2, 5, 19, -41,
        -35, -29, -22, -28, -30, -21, -16, -36,
};

int rook_table[64] = {
        26, 18, 15, 12, 16, 16, 0, 5,
        34, 26, 27, 23, 26, 26, 4, 16,
        30, 23, 22, 23, 23, 15, 15, 14,
        10, 20, 18, 18, 17, 14, -4, -3,
        -3, 0, 2, -3, 1, 0, -7, -21,
        -18, -11, 2, -7, -11, -20, -19, -48,
        -20, -9, -28, -13, -27, -34, -32, -95,
        -15, -7, 4, -5, 14, -7, -79, -56,
};

int queen_table[64] = {
        -4, 0, 20, 10, 17, 24, 12, 0,
        -17, -11, 3, 1, 13, 24, 24, 24,
        -17, -3, 9, 23, 23, 24, 23, 24,
        -23, -6, 6, 3, 6, 23, 10, 24,
        -11, -4, -5, -3, -1, 2, 19, 5,
        -2, -2, 4, -4, -8, 8, 14, 0,
        -21, -12, 2, -10, 0, 5, -26, -29,
        -7, -10, -8, 5, -46, -67, -32, -28,
};

int king_table[64] = {
        67, 65, 60, 78, 46, -44, -77, -265,
        101, 24, 27, 0, 10, -25, -17, -52,
        78, 40, 39, 31, -14, 25, -33, 1,
        7, 36, 24, 26, 29, 9, 21, -4,
        -29, 0, 9, 17, 12, 0, 1, -21,
        -33, -10, -9, -17, -19, -5, -7, -17,
        -3, 5, 2, -50, -54, -19, -9, -21,
        148, -14, -48, -54, -19, -42, -4, -30,
};

const int piece_values[] = {0, 119, 239, 272, 405, 768, 0};

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
