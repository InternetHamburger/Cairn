#include "evaluation.h"
#include "utility.h"
#include "board.h"

int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        312, 138, 89, 220, -56, 65, -194, 172,
        -32, -95, 144, 219, -75, -220, -123, -108,
        -39, -46, 119, 42, 0, -151, 26, -217,
        -70, -27, -29, -69, -11, 31, -21, -105,
        37, 6, -13, -47, 13, 86, -19, 9,
        36, 2, -16, -161, 11, 73, 33, 63,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int knight_table[64] = {
        138, -55, 22, -209, 58, -124, -316, -417,
        358, 81, 94, -225, 101, -186, -216, 196,
        235, 45, 25, 21, -93, -98, -197, 14,
        64, 17, 85, 1, -39, 51, -18, 246,
        -29, 130, 82, 40, 3, -63, 287, -10,
        -66, 103, -4, -12, 42, -34, 106, -85,
        26, 165, -140, 11, 49, 52, -211, -110,
        73, -14, 108, 27, -337, -31, 75, 109,
};

int bishop_table[64] = {
        -24, -60, -85, 65, -77, 179, -160, -21,
        -21, 22, 56, 32, -70, -127, 124, 58,
        -160, 79, -209, 11, -125, -61, 478, 60,
        -35, -116, 177, -83, 89, -9, 17, -157,
        78, 161, -175, -111, -46, 18, 45, 81,
        -34, -148, 100, -138, 13, -81, -5, -24,
        21, 116, 36, 100, -111, 122, -123, 245,
        100, 10, 16, 71, 112, -112, -117, -70,
};

int rook_table[64] = {
        -6, 38, 115, 92, 9, 101, 118, 105,
        -64, -48, 68, -125, -54, -79, -33, -113,
        3, 1, 69, 43, -19, -109, -18, -52,
        118, 134, 41, -64, -221, -77, 60, 26,
        46, 85, -178, -96, 137, 55, -227, -72,
        120, 26, -9, 29, 86, 37, 50, -49,
        -8, 130, 26, -9, 70, -30, 29, 168,
        11, -32, -74, -33, -126, -67, -113, -40,
};

int queen_table[64] = {
        19, -147, 103, -99, 167, 157, 23, 52,
        -176, 134, -70, -97, 210, -39, 110, 0,
        -199, -83, 83, 32, 84, -5, -198, 161,
        34, 162, -31, 76, 82, -123, 128, 10,
        -198, -82, 15, -4, 2, 76, 98, 214,
        -268, -94, 1, -78, -24, -41, 5, -28,
        209, -7, -9, 4, 20, 112, 6, -51,
        -158, 23, -64, -134, -152, 39, 24, -16,
};

int king_table[64] = {
        -110, -22, 39, 90, -157, -45, -61, 68,
        -239, -68, 219, 83, -16, 30, 205, 77,
        -97, 30, 28, 61, 35, 24, 250, -22,
        -96, -87, -34, -94, 84, 182, 75, 23,
        99, 30, 34, -33, 72, 0, 80, 101,
        5, -1, -14, -60, -84, 30, -4, -1,
        -11, -141, -107, 8, -33, -31, 41, -75,
        -8, -82, -130, -57, -157, -37, 116, -2,
};

const int piece_values[] = {0, 95, 294, 285, 426, 507, 0};


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
