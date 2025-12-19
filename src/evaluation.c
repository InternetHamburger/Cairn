#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        148, 152, 142, 157, 100, 116, 31, 125,
        61, 83, 57, 49, 41, 33, 8, 16,
        0, -1, -23, -33, -25, -33, -28, -42,
        -31, -32, -46, -31, -46, -52, -50, -57,
        -36, -33, -46, -52, -59, -70, -33, -56,
        -44, -25, -59, -83, -89, -24, -19, -64,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -54, -15, 0, 17, -13, -2, 8, -37,
        -50, 1, 24, 32, 52, 21, -22, 14,
        0, 28, 50, 56, 54, 60, 44, -14,
        -2, 10, 36, 50, 55, 66, 43, 51,
        -19, 0, 31, 27, 35, 33, 14, 11,
        -27, -21, -1, 23, 15, 19, 13, -31,
        -57, 0, -23, -6, -15, -5, -8, -35,
        -92, -43, -71, -53, -34, -44, -43, -164,
};

int bishop_table[64] = {
        -37, 2, -32, -7, -4, 0, 16, 5,
        -26, 5, 27, -1, 9, 21, -1, 0,
        17, 22, 25, 52, 14, 33, 26, 26,
        -9, 25, 34, 33, 41, 9, 21, 4,
        -21, 8, 37, 34, 25, 14, 4, -26,
        -17, 12, 3, 18, -2, 15, 7, 7,
        -14, -17, -8, -21, -4, -12, 19, -33,
        -46, -47, -40, -57, -44, -29, -84, -51,
};

int rook_table[64] = {
        6, 40, 31, 24, 22, 30, 21, 26,
        51, 43, 60, 54, 49, 36, 32, 26,
        41, 41, 31, 30, 37, 42, 40, 46,
        17, 21, 11, 15, 4, 23, 5, 17,
        -6, -7, 2, -8, 0, 4, -6, -28,
        -34, -26, -29, -26, -26, -26, -40, -46,
        -54, -23, -48, -43, -51, -55, -70, -97,
        -28, -23, -17, -13, 0, -25, -63, -63,
};

int queen_table[64] = {
        12, 12, 12, 12, 12, 12, 12, 12,
        11, 12, 12, 12, 12, 12, 12, 12,
        12, 12, 12, 12, 12, 12, 12, 12,
        -2, 12, 12, 12, 12, 12, 12, 12,
        -4, 12, 12, 12, 12, 12, 12, 12,
        -19, 4, 11, 11, 5, 12, 12, 6,
        -16, -14, 5, -8, -6, 0, -20, -38,
        -33, -22, -34, 2, -46, -65, -173, -21,
};

int king_table[64] = {
        7, 18, -11, -35, 14, -13, 33, -54,
        -19, 28, 22, 28, 17, 16, 24, -33,
        34, 57, 40, 33, 34, 59, 62, 22,
        -54, 24, 21, 39, 58, 40, 57, 8,
        -30, -17, 0, 13, 13, 32, 21, -6,
        -65, -42, -47, -28, -10, 1, 4, -10,
        -37, -23, -17, -54, -54, -7, 0, -11,
        5, -19, -55, -73, -26, -45, 23, -16,
};

const int piece_values[] = {0, 176, 340, 372, 543, 780, 0};

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
