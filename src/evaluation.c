#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        44, 80, 84, 79, 90, 93, -95, 99,
        5, 95, 44, -12, 15, -18, -44, -24,
        6, 15, 6, -18, -35, -41, -45, -24,
        -6, -8, 6, -38, 2, -39, -46, -44,
        6, -9, 8, -18, -24, -34, -24, -30,
        -3, -17, -38, -12, 24, -20, -10, -26,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -83, 41, 4, 52, 54, 82, -43, -104,
        -21, 26, -30, 47, -21, 12, 16, 64,
        -2, 20, 29, 1, 31, -29, 63, 41,
        23, -57, -1, -3, 34, 0, 2, -4,
        53, -9, -1, -4, 23, 16, -30, 19,
        -35, 8, 19, -44, -7, -8, 31, 62,
        -79, 3, 61, -27, 9, -56, 65, -61,
        -146, 22, -137, -89, 30, 31, 30, 8,
};

int mg_bishop_table[64] = {
        6, -9, 24, -27, -77, -44, 14, -78,
        25, -41, -28, -3, -28, 26, -31, 43,
        -58, 47, -3, -26, 33, -18, -21, -5,
        43, 17, -24, 32, 13, 23, 13, -13,
        37, 8, 26, 5, 33, 9, -20, -29,
        -7, -4, 30, 30, 6, -1, -45, 23,
        20, -22, 27, 18, -16, 25, 52, 23,
        55, 14, 3, -5, -35, -2, -144, 59,
};

int mg_rook_table[64] = {
        -5, -31, 0, -2, -38, 25, -3, 32,
        15, 14, 61, 24, 9, 11, 16, 63,
        7, 2, 26, 32, -37, -25, -7, -5,
        13, 9, 11, 18, 36, 20, 12, -14,
        -17, -17, -4, 18, -3, -12, -28, 2,
        21, 6, 15, 7, 20, -29, -5, -19,
        20, 20, -7, -15, -41, -9, -45, -42,
        -14, -30, 29, -31, 1, -52, 6, -34,
};

int mg_queen_table[64] = {
        -1, 12, 13, 12, 9, 13, 13, 13,
        -42, 13, 12, 12, 13, 13, 13, 11,
        -1, 11, 12, 2, 13, -7, 13, -43,
        -7, 13, 13, 13, 13, 13, 13, 12,
        -5, -9, -5, 13, 10, 13, -22, 7,
        -33, 12, -4, 9, 13, 4, 8, 12,
        -44, -21, 13, 4, 13, -61, -24, 3,
        13, -21, -19, 13, -3, -26, -72, 0,
};

int mg_king_table[64] = {
        13, -150, -123, -85, -42, 160, -16, -43,
        -155, 51, -119, -96, -14, 181, -97, 24,
        124, -22, -30, -61, -43, 97, -4, 43,
        -50, -97, -33, 42, -108, -49, 34, -32,
        -33, -29, -62, -48, -48, 33, 77, 21,
        28, -24, -29, -110, 22, 31, 45, 82,
        73, 33, 33, 31, 15, 49, 66, 49,
        65, 74, 24, 15, 67, 49, 75, 26,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        97, 265, 251, 233, 252, 5, -93, 158,
        -54, 55, 74, -5, -9, 24, -74, -26,
        -38, -9, 20, -5, -27, -48, -65, -36,
        -64, -30, -29, -10, -37, -58, -54, -55,
        -44, -47, -31, -38, -66, -46, 6, -48,
        -55, -32, -26, -36, -42, -28, -19, -55,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        42, 145, -132, 71, 23, -41, -67, -81,
        -18, -41, 62, -32, -14, 53, -23, -3,
        -90, 124, 11, 64, 98, 104, 96, 87,
        28, -26, 12, 62, 28, 24, 26, 63,
        -13, 51, 0, 4, 20, 7, 6, -13,
        -16, 12, -4, -21, -64, 6, -21, 6,
        -46, 77, 5, -9, -22, 21, -272, -11,
        -93, -37, -385, -104, 147, 21, -22, 112,
};

int eg_bishop_table[64] = {
        -4, -94, -51, 123, -473, 4, -101, 88,
        31, 9, 61, 151, 116, 85, -5, 25,
        -88, 204, 38, 54, 147, 53, -46, -11,
        -25, 8, 23, 28, 45, 30, 41, 9,
        -11, 4, 5, -3, 54, 21, -22, 16,
        0, -17, -3, 13, 0, -9, 11, 14,
        -6, -16, -12, -2, -33, 1, 30, -166,
        50, 4, -12, -131, -86, -8, -187, 23,
};

int eg_rook_table[64] = {
        82, 96, 111, 79, -348, 119, 109, 105,
        -22, 15, 156, -117, -119, 148, -149, 84,
        108, -6, 114, 90, -48, 31, -105, 36,
        37, 5, 76, 24, 4, -22, -34, 10,
        -20, -9, -58, -1, 6, 27, -17, 25,
        -39, -75, -53, -92, 0, -8, -22, -51,
        -16, 3, -64, 57, -7, 25, -124, -6,
        -13, -5, -11, 3, -9, -3, -62, -43,
};

int eg_queen_table[64] = {
        10, 40, 39, -58, 33, -118, 40, 40,
        30, 39, -36, -196, -3, -4, 36, -23,
        -8, 39, -16, 21, 40, 10, 40, 39,
        40, 40, 40, 13, -5, -13, 39, 40,
        34, 28, 21, 39, 37, 6, 23, 14,
        -17, 35, 8, 38, 2, 36, -3, -66,
        -295, 25, 35, 34, 31, -31, -99, -5,
        26, 12, 40, 37, 40, -7, -339, 35,
};

int eg_king_table[64] = {
        46, -28, -8, -35, -35, 85, -102, 22,
        -35, 38, -35, -35, 1, 84, -151, -119,
        65, -120, -108, -17, -13, 58, -62, 43,
        -135, -163, -362, -15, 130, 224, -186, -122,
        -46, 199, -87, -21, 192, -46, 8, -59,
        9, -11, -22, -96, 57, 46, -40, 83,
        107, 118, 31, 7, 2, 21, 65, -3,
        108, 124, 77, 23, 68, 30, 66, 79,
};

const int mg_piece_values[] = {0, 100, 134, 149, 269, 470, 0};
const int eg_piece_values[] = {0, 94, 184, 207, 284, 443, 0};

int mg_piece_square_values[BlackKing + 1][64];
int eg_piece_square_values[BlackKing + 1][64];

__attribute__((constructor))
static void init_table(void) {
    for (int i = 0; i < 64; i++)
    {
        mg_piece_square_values[WhitePawn][i] = mg_pawn_table[i] + mg_piece_values[Pawn];
        mg_piece_square_values[WhiteKnight][i] = mg_knight_table[i] + mg_piece_values[Knight];
        mg_piece_square_values[WhiteBishop][i] = mg_bishop_table[i] + mg_piece_values[Bishop];
        mg_piece_square_values[WhiteRook][i] = mg_rook_table[i] + mg_piece_values[Rook];
        mg_piece_square_values[WhiteQueen][i] = mg_queen_table[i] + mg_piece_values[Queen];
        mg_piece_square_values[WhiteKing][i] = mg_king_table[i] + mg_piece_values[King];

        mg_piece_square_values[BlackPawn][i] = -mg_pawn_table[FlipSquare(i)] - mg_piece_values[Pawn];
        mg_piece_square_values[BlackKnight][i] = -mg_knight_table[FlipSquare(i)] - mg_piece_values[Knight];
        mg_piece_square_values[BlackBishop][i] = -mg_bishop_table[FlipSquare(i)] - mg_piece_values[Bishop];
        mg_piece_square_values[BlackRook][i] = -mg_rook_table[FlipSquare(i)] - mg_piece_values[Rook];
        mg_piece_square_values[BlackQueen][i] = -mg_queen_table[FlipSquare(i)] - mg_piece_values[Queen];
        mg_piece_square_values[BlackKing][i] = -mg_king_table[FlipSquare(i)] - mg_piece_values[King];

        eg_piece_square_values[WhitePawn][i] = eg_pawn_table[i] + eg_piece_values[Pawn];
        eg_piece_square_values[WhiteKnight][i] = eg_knight_table[i] + eg_piece_values[Knight];
        eg_piece_square_values[WhiteBishop][i] = eg_bishop_table[i] + eg_piece_values[Bishop];
        eg_piece_square_values[WhiteRook][i] = eg_rook_table[i] + eg_piece_values[Rook];
        eg_piece_square_values[WhiteQueen][i] = eg_queen_table[i] + eg_piece_values[Queen];
        eg_piece_square_values[WhiteKing][i] = eg_king_table[i] + eg_piece_values[King];

        eg_piece_square_values[BlackPawn][i] = -eg_pawn_table[FlipSquare(i)] - eg_piece_values[Pawn];
        eg_piece_square_values[BlackKnight][i] = -eg_knight_table[FlipSquare(i)] - eg_piece_values[Knight];
        eg_piece_square_values[BlackBishop][i] = -eg_bishop_table[FlipSquare(i)] - eg_piece_values[Bishop];
        eg_piece_square_values[BlackRook][i] = -eg_rook_table[FlipSquare(i)] - eg_piece_values[Rook];
        eg_piece_square_values[BlackQueen][i] = -eg_queen_table[FlipSquare(i)] - eg_piece_values[Queen];
        eg_piece_square_values[BlackKing][i] = -eg_king_table[FlipSquare(i)] - eg_piece_values[King];
    }
}

int eval(const Board *board) {
    uint64_t occupied = GetOccupied(board);

    int mg_eval = 0;
    int eg_eval = 0;
    while (occupied){
        const int square = poplsb(&occupied);
        mg_eval += mg_piece_square_values[board->squares[square]][square];
        eg_eval += eg_piece_square_values[board->squares[square]][square];
    }

    // Interpolate based on number of pieces
    int num_pieces = __builtin_popcountll(occupied);
    int eval = (mg_eval * num_pieces + eg_eval * (32 - num_pieces)) / 32;

    return eval * (board->white_to_move ? 1 : -1);
}
