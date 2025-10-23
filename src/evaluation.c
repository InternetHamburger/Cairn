#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        182, 181, 200, 161, 115, 156, 104, 117,
        92, 106, 101, 75, 34, 6, 36, 70,
        12, -19, -27, -37, -39, -44, -33, -46,
        -21, -48, -56, -30, -56, -81, -83, -76,
        -27, -28, -56, -75, -89, -121, -47, -63,
        -54, -32, -87, -114, -110, -52, -25, -69,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -48, -97, -17, -2, -12, -51, -9, 35,
        -63, 29, 34, 36, -12, 27, -35, -37,
        -20, 75, 59, 68, -38, 13, 69, -26,
        6, 4, 42, 74, 87, 64, 43, 31,
        -24, -1, 52, 59, 50, 39, 9, 7,
        -27, -28, 21, 24, 31, 47, 20, -15,
        -39, 7, 34, 11, -6, -15, -81, -35,
        -51, -35, -116, -68, -49, -84, -28, -47,
};

int bishop_table[64] = {
        35, 19, -35, 30, -25, -3, -57, 26,
        -50, -1, 35, 12, 17, 3, -1, -34,
        -7, 42, 19, 65, -2, 50, 23, 37,
        18, 27, 35, 38, 54, 32, 31, 11,
        -22, 37, 44, 65, 26, 21, -22, -24,
        2, 22, 19, 24, -13, 3, -53, 37,
        -18, -8, -26, -17, -8, 0, 38, -78,
        -61, -71, -39, -67, -80, -32, -85, -62,
};

int rook_table[64] = {
        55, 41, 33, 26, 25, 37, 6, 24,
        71, 57, 52, 50, 45, 48, 22, 38,
        67, 53, 57, 54, 53, 47, 54, 41,
        31, 48, 38, 40, 34, 33, -3, 11,
        -3, 1, 0, -12, -2, -5, -26, -22,
        -32, -25, -1, -38, -49, -70, -55, -72,
        -38, -17, -73, -60, -86, -94, -57, -163,
        -15, -17, 2, -28, 29, -13, -137, -85,
};

int queen_table[64] = {
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        1, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 1, 2, 2, 2, 2,
        2, 2, 2, -1, 2, 2, -9, 2,
        2, 2, 1, 2, -30, -71, 0, 1,
};

int king_table[64] = {
        122, 45, 64, 82, 62, -33, -74, -280,
        114, 31, 5, -21, 17, -18, -2, -36,
        102, 71, 59, 39, 5, 49, 2, 10,
        30, 54, 32, 51, 55, 40, 38, 4,
        -85, -6, 6, 31, 35, 1, 13, -51,
        -70, -38, -10, -36, -40, 0, -15, -47,
        -34, 31, 12, -83, -86, -13, 2, -46,
        158, -63, -101, -99, 2, -57, 37, -44,
};

const int piece_values[] = {0, 204, 335, 380, 537, 790, 0};

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
