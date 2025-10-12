#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        178, 213, 175, 216, 124, 163, 138, 98,
        16, 111, 71, 125, 48, 0, 64, 46,
        -19, 14, -13, 7, 17, -25, 5, -74,
        -77, -34, -45, -3, -47, -87, -64, -93,
        -58, -34, -45, -68, -93, -161, -39, -80,
        -87, -23, -111, -143, -162, -54, -10, -95,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -125, -50, -42, 21, -24, -167, 34, 11,
        -59, -1, 34, 31, 0, 11, -90, -44,
        -66, 34, 52, 53, 72, -7, 91, -43,
        34, 14, 5, 101, 97, 51, 54, 76,
        -24, -4, 36, 10, 34, 31, 2, 37,
        -41, -36, 16, 43, 76, 58, 35, -39,
        55, 21, 39, 8, 13, 14, -33, 0,
        -125, -71, -97, -62, -22, -48, -44, -73,
};

int bishop_table[64] = {
        -10, -36, -35, 15, -75, 79, 10, 27,
        -34, 15, 15, 17, 40, -23, 0, -23,
        -16, 93, -30, 30, -20, 47, 17, 22,
        17, 8, 38, 50, 48, 7, 16, 4,
        -11, 47, 47, 32, 21, -7, -39, -15,
        -7, 16, 25, 8, -5, -6, -75, 34,
        88, -17, 0, -41, -27, 13, -9, -59,
        -2, 41, -65, -17, -72, -64, -121, -47,
};

int rook_table[64] = {
        52, 23, 56, 26, -1, 19, -4, 58,
        25, 27, 10, 29, 31, 63, 12, 15,
        23, 37, 58, 23, 9, 60, -6, -1,
        21, 9, 28, 30, 35, 23, 6, 25,
        3, 8, 23, -5, -2, 15, -34, -26,
        -8, -24, 23, -38, -25, -65, -61, -59,
        -15, -6, -31, -14, -38, -51, -22, -170,
        -16, -14, 0, -15, 11, -3, -89, -83,
};

int queen_table[64] = {
        -19, 11, 11, 11, 11, 11, 11, 11,
        -12, 11, 11, 11, 11, 11, 11, 10,
        11, 11, 11, 11, 11, 11, 11, 11,
        -5, 11, 11, 11, 11, 11, 11, 11,
        -5, -10, 11, 11, 11, 11, 11, 11,
        -73, 11, -2, 11, -14, 11, 11, 11,
        11, -15, -16, -7, 9, -22, -78, 5,
        -50, -26, 11, -1, -36, -3, 10, -113,
};

int king_table[64] = {
        -1, 1, 88, 24, -21, -5, 3, 3,
        8, 33, 85, 0, 26, 9, 30, -10,
        15, 34, 56, 45, 23, 114, 55, -37,
        70, 87, 59, 18, 53, 27, 5, -17,
        3, 18, 40, 54, 13, -22, 0, -48,
        12, -23, 11, -43, -42, -15, -18, -36,
        -70, 20, -12, -101, -88, -35, 4, -52,
        -104, -94, -139, -92, 14, -42, 29, -38,
};

const int piece_values[] = {0, 167, 340, 397, 515, 780, 0};

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
