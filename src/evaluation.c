#include "evaluation.h"
#include "utility.h"
#include "board.h"

int mg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        106, 120, 97, 87, 38, -9, 62, 29,
        34, 74, 57, 32, 36, -13, 5, 55,
        13, 10, 5, -9, 16, -22, -8, -10,
        -33, -8, -14, -8, -29, -63, -53, -50,
        0, -12, 2, -64, -49, -88, -26, -35,
        -29, 0, -58, -19, -90, -26, -30, -24,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int mg_knight_table[64] = {
        -22, -52, 23, 28, 34, -75, -52, -104,
        -80, 49, 45, 34, -5, 55, -22, -55,
        -11, 71, 53, 41, 49, -10, 17, -46,
        26, 63, 13, 38, 58, 56, 20, 20,
        -34, 11, 53, 8, -24, 41, 6, 6,
        33, -2, -11, -8, 29, 25, -1, 15,
        22, -5, 8, 21, -40, -25, -166, -110,
        -39, -7, 78, -10, 6, -31, -41, -68,
};

int mg_bishop_table[64] = {
        72, -43, 8, -14, 61, 25, -34, -8,
        -32, -3, 13, 32, 8, -25, 1, 3,
        -7, -15, -7, -2, -13, 48, -14, 11,
        14, 20, -11, 10, 51, 11, 25, 32,
        37, 19, 19, 3, 22, -9, -42, -14,
        24, 22, 39, 38, 13, -54, -25, 40,
        20, -19, 0, -59, -36, -27, 81, -139,
        -33, -15, 9, 8, -67, -30, -99, 54,
};

int mg_rook_table[64] = {
        3, -7, 19, -16, -9, 2, -36, -12,
        39, 26, 22, 21, -4, 14, 12, 21,
        -3, 15, 16, 3, -1, 15, 10, 1,
        20, 43, -26, 9, 27, 12, 6, 21,
        2, 12, 5, 20, -15, 10, -6, -13,
        11, -31, 2, 4, 15, -48, 14, -7,
        -4, 8, -8, -8, -38, -23, -26, -9,
        -16, -19, -20, 1, -16, -8, -21, -34,
};

int mg_queen_table[64] = {
        -9, 2, 2, 2, 2, 2, 2, 2,
        -6, 2, 2, 2, 2, 2, 2, -4,
        2, 0, 2, 2, 2, 2, 2, 2,
        -8, 2, 2, 2, 2, 1, 2, 2,
        2, -1, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, -1, 2, -1, -14,
        2, -5, 2, 2, 1, -1, -20, 2,
        2, -1, 2, 2, -30, -2, 2, 2,
};

int mg_king_table[64] = {
        -83, 6, -23, -49, -141, 38, 59, 32,
        -180, -63, -53, -89, -20, 47, 105, 52,
        -69, 6, -23, -11, -15, -10, 16, 55,
        13, 45, 9, -27, 43, 47, 43, 40,
        -25, -1, 6, 29, -5, -15, 31, -11,
        -4, -17, -6, -13, 12, 22, 8, 26,
        67, -2, 11, -33, -14, 11, 30, 9,
        -34, 26, -5, -2, 44, 5, 47, 0,
};

int eg_pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        -21, -8, 166, 98, -205, 134, -53, 158,
        -4, 81, 50, 49, 40, -28, -27, 21,
        -11, 15, 6, 16, -5, -12, -34, 5,
        -30, -11, -8, 3, -13, -28, -24, -40,
        -15, -18, -7, -17, -31, -34, -2, -31,
        -17, -6, -17, -24, -39, -10, 4, -17,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int eg_knight_table[64] = {
        -43, -115, 40, -57, -80, -82, -147, -40,
        -48, 89, -28, -1, 69, 41, 94, 9,
        102, 69, 129, 124, 35, -25, 68, -34,
        -21, 29, 11, 58, 67, 39, 57, 67,
        16, 5, 41, 40, 33, 36, 17, -11,
        -1, 15, 18, 33, 14, 34, -2, 18,
        -15, -1, 6, 16, 7, 4, -111, -89,
        -369, 7, -107, -7, -112, 0, -15, 7,
};

int eg_bishop_table[64] = {
        77, -98, -166, -196, -3, 116, 14, 68,
        -228, -1, -7, -97, 50, 21, -32, 11,
        32, -2, -38, -35, -51, 4, -96, 57,
        1, 46, 36, 41, 78, 73, 38, 52,
        49, 50, 40, 49, 61, 41, -13, -46,
        19, 8, 66, 41, 40, -7, -80, 17,
        52, 9, 43, 33, -12, 34, 57, -53,
        -10, 47, 31, -35, -23, 5, -302, 25,
};

int eg_rook_table[64] = {
        -82, -229, -237, -75, -89, 116, -131, 6,
        116, 24, 126, -47, -60, -36, 7, 21,
        38, 40, -81, 123, -54, -6, 86, 74,
        110, 114, -105, 64, 85, 104, 45, 49,
        22, -10, -42, 107, -258, 20, 16, 60,
        -5, -33, -20, -12, 22, -48, 17, -31,
        18, 99, 23, 10, -10, 61, -52, -53,
        6, 18, 6, -2, -2, -3, -46, 3,
};

int eg_queen_table[64] = {
        -15, -22, 18, 18, 18, 18, 18, 17,
        18, 18, 18, 18, 18, 18, 6, 15,
        18, -16, 18, -10, -53, 18, -77, -6,
        -3, -18, 17, 18, 18, 15, 2, 12,
        7, 9, 4, 7, -16, -7, 9, 16,
        -76, 18, 18, 9, 11, 18, 12, 3,
        -37, 3, 15, 11, 9, -41, -25, 18,
        18, -54, 18, 15, -51, -33, -79, 18,
};

int eg_king_table[64] = {
        -21, 35, 25, -67, -21, 51, 69, 40,
        -128, -94, -77, -97, 28, 50, 72, 34,
        -84, 41, -72, -22, -68, 38, 21, 38,
        41, 115, 186, -247, -98, -108, 104, -86,
        -17, 26, -36, -109, -174, -80, 83, -156,
        -35, -95, -57, -155, -57, -10, -11, 202,
        293, 76, 38, 2, -8, 41, 83, 46,
        149, 192, -130, -44, 56, 42, 117, 30,
};

const int mg_piece_values[] = {0, 100, 136, 160, 243, 346, 0};
const int eg_piece_values[] = {0, 62, 153, 156, 197, 330, 0};

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
