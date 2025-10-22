#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        229, 232, 213, 215, 136, 167, 134, 78,
        87, 104, 98, 96, 36, 17, 43, 51,
        5, -6, -11, -41, -39, -46, -33, -55,
        -41, -59, -62, -37, -67, -88, -88, -92,
        -37, -46, -55, -80, -102, -140, -42, -75,
        -54, -46, -102, -119, -138, -50, -19, -84,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -126, -29, -54, 52, -57, -39, -63, 52,
        -96, 23, 60, 31, 24, 52, -59, -117,
        -52, 64, 56, 97, 99, 8, 48, -22,
        4, 21, 50, 71, 98, 70, 66, 0,
        -36, -21, 53, 48, 52, 41, -22, 26,
        -19, -30, 13, 43, 47, 55, 17, 5,
        -66, 1, 9, 14, -2, -3, -59, -5,
        -79, -27, -73, -32, -74, -52, -14, -140,
};

int bishop_table[64] = {
        5, -27, -13, 28, -29, -1, -30, 20,
        -68, 22, 10, 2, 27, 17, -6, -12,
        -36, 36, 1, 51, 1, 51, 20, 7,
        7, 23, 34, 40, 35, 31, 34, 26,
        -21, 32, 50, 63, 34, 21, -17, -7,
        -5, 23, 20, 13, 4, 9, -49, 14,
        12, 0, -40, -10, -12, 27, 38, -70,
        -73, -32, -27, -31, -62, -40, -100, -45,
};

int rook_table[64] = {
        49, 31, 38, 27, 15, 30, 5, 54,
        49, 53, 38, 47, 33, 44, 23, 34,
        47, 51, 45, 39, 37, 39, 35, 28,
        26, 51, 40, 31, 39, 32, 4, 28,
        -11, 0, -3, -17, 2, 8, -39, -5,
        -40, -22, -5, -41, -41, -66, -68, -54,
        -38, 0, -65, -45, -66, -92, -51, -154,
        -25, -20, -9, -26, 15, -16, -119, -59,
};

int queen_table[64] = {
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        0, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        -10, 1, 2, 1, 2, 2, -6, 0,
        0, -17, -9, 2, -28, -58, 1, -2,
};

int king_table[64] = {
        0, -5, -63, -32, -31, 64, 88, 116,
        5, 28, -14, -5, 20, 19, 87, 101,
        13, 41, 40, 23, 58, -36, 107, 20,
        37, 23, 32, 35, 48, 66, 36, 24,
        -41, 3, 7, 35, 32, 1, 17, -63,
        -35, -39, -1, -33, -36, 6, 0, -33,
        -84, 15, 6, -83, -72, -1, 29, -40,
        -215, -136, -129, -91, 8, -28, 64, -9,
};

const int piece_values[] = {0, 222, 353, 399, 597, 789, 0};

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
