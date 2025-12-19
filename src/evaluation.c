#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        144, 149, 150, 127, 92, 92, 59, 60,
        58, 67, 57, 53, 36, 18, 24, 32,
        -3, -6, -20, -23, -21, -36, -28, -36,
        -23, -28, -40, -23, -39, -48, -48, -55,
        -27, -24, -34, -51, -52, -72, -26, -51,
        -42, -24, -61, -80, -85, -28, -19, -60,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -60, -7, -5, 3, 0, -42, -43, -9,
        -40, 10, 32, 32, 8, 35, -26, 5,
        -10, 52, 43, 52, 51, 31, 44, -1,
        -13, 9, 29, 51, 50, 41, 27, 22,
        -18, 11, 34, 31, 35, 27, 16, 2,
        -23, -13, 8, 18, 14, 25, 18, -22,
        -49, -19, -4, 0, -9, -13, -32, -34,
        -55, -32, -71, -31, -39, -32, -32, -85,
};

int bishop_table[64] = {
        17, -11, -17, -3, -4, -17, -34, 2,
        -29, 5, 18, 2, 8, 0, -2, -24,
        -11, 21, 21, 33, 24, 31, 24, 22,
        -6, 25, 26, 32, 33, 20, 16, 2,
        -11, 15, 28, 35, 22, 16, -4, -13,
        -2, 10, 11, 15, 0, 16, -4, 4,
        -24, -5, -5, -8, -2, 2, 23, -43,
        -35, -48, -27, -31, -29, -33, -53, -35,
};

int rook_table[64] = {
        21, 23, 24, 25, 24, 14, 15, 21,
        31, 32, 32, 29, 29, 28, 18, 20,
        29, 28, 25, 27, 26, 27, 25, 32,
        11, 18, 19, 18, 13, 22, 7, 4,
        -5, -3, 1, -5, 2, -2, -8, -19,
        -27, -13, -8, -13, -22, -27, -24, -46,
        -32, -21, -34, -22, -34, -44, -36, -97,
        -22, -16, -8, -6, 6, -16, -72, -54,
};

int queen_table[64] = {
        9, 9, 9, 9, 9, 9, 9, 9,
        -8, 9, 9, 9, 9, 9, 9, 9,
        2, 9, 9, 9, 9, 9, 9, 9,
        -2, 5, 9, 9, 9, 9, 9, 9,
        -3, 5, 9, 9, 9, 9, 9, 8,
        -11, 3, 9, 4, -4, 8, 9, -8,
        -17, -9, 6, -5, -3, 0, -29, -23,
        -12, -23, -16, 5, -47, -80, -57, -43,
};

int king_table[64] = {
        -30, 37, -12, -43, 12, 7, 47, -93,
        -52, 11, 11, 32, 20, 41, 51, -12,
        -20, 10, 27, 30, 54, 46, 74, 25,
        -2, 11, 15, 25, 35, 39, 44, 9,
        -31, -6, -2, 10, 17, 17, 13, -9,
        -41, -23, -25, -27, -18, 0, 3, -6,
        -36, -15, -15, -56, -53, -3, 12, -2,
        20, -23, -61, -70, -8, -36, 26, -11,
};

const int piece_values[] = {0, 152, 310, 344, 497, 782, 0};

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
