#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        243, 251, 228, 235, 146, 181, 135, 96,
        92, 117, 102, 111, 42, 19, 55, 56,
        7, -1, -10, -40, -28, -45, -28, -64,
        -48, -60, -63, -32, -69, -94, -92, -99,
        -44, -50, -60, -85, -118, -163, -47, -81,
        -63, -42, -115, -139, -161, -56, -18, -93,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -125, -42, -62, 46, -65, -55, -74, 60,
        -93, 20, 65, 26, 23, 52, -52, -96,
        -48, 64, 59, 98, 99, 3, 59, -33,
        11, 17, 52, 85, 110, 79, 68, 10,
        -34, -14, 54, 46, 51, 42, -17, 26,
        -24, -25, 18, 45, 53, 63, 20, -11,
        -52, 3, 14, 16, -6, 0, -63, -19,
        -85, -36, -87, -41, -65, -62, -23, -140,
};

int bishop_table[64] = {
        3, -39, -25, 29, -40, 11, -29, 32,
        -75, 23, 17, 5, 28, 14, -8, -12,
        -31, 49, -3, 49, -1, 51, 25, 7,
        4, 21, 38, 44, 39, 30, 36, 19,
        -19, 38, 52, 58, 30, 24, -26, -10,
        -5, 24, 24, 12, 1, 0, -63, 15,
        29, 0, -37, -23, -17, 24, 33, -76,
        -69, -22, -35, -39, -68, -48, -90, -53,
};

int rook_table[64] = {
        52, 32, 47, 30, 14, 32, 15, 59,
        56, 58, 44, 48, 43, 51, 24, 40,
        46, 54, 49, 39, 47, 47, 39, 31,
        29, 47, 40, 32, 40, 34, 5, 40,
        -5, 0, 8, -13, -3, 9, -40, -7,
        -37, -19, -4, -47, -48, -70, -69, -62,
        -36, -6, -70, -53, -64, -100, -54, -181,
        -26, -20, -9, -30, 18, -17, -124, -66,
};

int queen_table[64] = {
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 1, 2, 2, 2, 2, 2,
        0, 2, 2, 2, 2, 1, -17, 1,
        1, -25, -5, 2, -27, -49, 1, 2,
};

int king_table[64] = {
        0, -5, -16, -14, -17, 63, 64, 76,
        -9, 36, 12, 1, 25, 18, 75, 74,
        17, 51, 46, 36, 48, 7, 101, 11,
        45, 38, 35, 32, 56, 63, 39, 6,
        -28, 11, 20, 43, 35, -3, 15, -65,
        -30, -42, -1, -31, -45, 7, -6, -36,
        -91, 18, 0, -96, -79, -8, 24, -50,
        -228, -149, -143, -103, 11, -32, 69, -17,
};

const int piece_values[] = {0, 238, 379, 432, 631, 790, 0};

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
