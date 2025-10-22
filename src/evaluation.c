#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        236, 238, 220, 224, 141, 173, 140, 86,
        88, 111, 100, 104, 38, 18, 46, 54,
        4, -3, -10, -38, -34, -45, -29, -58,
        -45, -59, -63, -34, -67, -92, -90, -96,
        -40, -48, -57, -83, -108, -152, -43, -79,
        -59, -44, -109, -130, -151, -51, -17, -88,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        -127, -33, -56, 52, -58, -43, -61, 56,
        -95, 21, 62, 32, 22, 51, -61, -111,
        -53, 66, 58, 99, 101, 8, 54, -25,
        7, 22, 50, 78, 103, 72, 67, 5,
        -37, -20, 55, 48, 53, 41, -22, 27,
        -23, -32, 15, 44, 50, 59, 18, 0,
        -59, 3, 10, 13, -3, -3, -63, -9,
        -85, -32, -77, -36, -75, -57, -18, -140,
};

int bishop_table[64] = {
        3, -27, -16, 29, -34, 4, -28, 23,
        -69, 20, 12, 1, 27, 18, -5, -15,
        -37, 42, 0, 51, 0, 52, 20, 8,
        7, 22, 35, 42, 38, 30, 35, 23,
        -22, 33, 52, 63, 33, 20, -21, -8,
        -6, 23, 20, 13, 2, 6, -56, 16,
        19, -1, -40, -14, -17, 25, 35, -73,
        -72, -28, -32, -35, -69, -45, -96, -49,
};

int rook_table[64] = {
        52, 33, 41, 28, 16, 32, 8, 56,
        51, 54, 41, 49, 36, 48, 24, 35,
        48, 53, 48, 41, 38, 42, 35, 29,
        28, 52, 41, 33, 41, 34, 5, 31,
        -9, 1, -1, -17, 1, 8, -41, -4,
        -40, -23, -4, -43, -44, -68, -72, -57,
        -38, 0, -65, -48, -68, -95, -53, -164,
        -24, -21, -9, -27, 15, -16, -122, -63,
};

int queen_table[64] = {
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        1, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        -8, 1, 2, 1, 2, 2, -1, 2,
        0, -16, -8, 2, -29, -57, 1, 0,
};

int king_table[64] = {
        0, -2, -40, -24, -29, 61, 76, 96,
        0, 31, -2, 0, 21, 18, 84, 84,
        13, 44, 42, 27, 55, -15, 105, 15,
        44, 32, 35, 36, 52, 65, 35, 19,
        -39, 6, 11, 39, 32, 0, 14, -66,
        -34, -40, 0, -34, -38, 3, -3, -37,
        -89, 15, 2, -88, -78, -4, 26, -44,
        -220, -143, -137, -97, 9, -31, 66, -12,
};

const int piece_values[] = {0, 229, 366, 415, 613, 790, 0};

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
